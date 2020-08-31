#pragma once
#include <mutex>
#include <chrono>
#include <sstream>
#include <functional>
#include <thread>
#include <mutex>

/**
 * @brief �ֲ�ʽid������
 * 64bit id: 0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
 *           ||                                                           ||     ||     |  |              |
 *           |��---------------------------ʱ���--------------------------��������-��������-��  ��----���к�----��
 *           |
 *         ����
 * SnowFlake���ŵ�: �����ϰ���ʱ����������, ���������ֲ�ʽϵͳ�ڲ������ID��ײ(����������ID�ͻ���ID������),
 * ����Ч�ʽϸ�, ������, SnowFlakeÿ���ܹ�����400�����ҵ�Id.
 */

class IdWorker
{
public:
    IdWorker(int32_t masterId, int32_t NodeId)
    {
        id_.id = 0;
        id_.st.masterId = masterId;
        id_.st.NodeId = NodeId;
    }

    int64_t CreateId()
    {
        auto timestamp = GetMillionSecond();
        std::unique_lock<std::mutex> lock{ mutex_ };

        if (timestamp < last_timestamp_)
        {//���ֵ�ǰʱ��С����һ��ID����ʱ�䣬˵��ϵͳʱ�ӷ�������
            auto offset = last_timestamp_ - timestamp;
            if (offset <= 10)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(offset * 2));

                timestamp = GetMillionSecond();
            }
            else
            {
                id_.st.timeRollBack++;
            }
        }

        if (last_timestamp_ == timestamp)
        {
            //ͬʱ���ɣ���������
            id_.st.sequence++;
            if (0 == id_.st.sequence)
            {
                //�������,��һ����
                timestamp = WaitMillionSecond(last_timestamp_);
            }
        }
        else
            id_.st.sequence = 0;

        last_timestamp_ = timestamp;
        id_.st.timestamp = timestamp - START_TIME;
        return id_.id;
    }

private:
    int64_t GetMillionSecond() const
    {
        auto timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now());
        return timestamp.time_since_epoch().count();
    }

    int64_t WaitMillionSecond(int64_t last_timestamp) const
    {
        auto now = GetMillionSecond();
        while (now <= last_timestamp)
        {
            now = GetMillionSecond();
        }
        return now;
    }

private:
    struct StId {
        int64_t sequence : 10;
        int64_t NodeId : 5;
        int64_t masterId : 5;
        int64_t timeRollBack : 5;
        int64_t timestamp : 38;
        int64_t unused : 1;
    };

    union UnionId
    {
        StId st;
        int64_t id;
    };

    const int64_t START_TIME = 1577808000000; //2020-1-1:0:0:0

    std::mutex mutex_;

    int64_t last_timestamp_ = 0;
    UnionId id_;
};