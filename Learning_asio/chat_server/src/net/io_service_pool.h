#pragma once
#include <asio.hpp>
#include <thread>
#include <mutex>

class IOContextPool {
public:
	using IOCPtr = std::shared_ptr<asio::io_context>;
	using WorkPtr = std::shared_ptr<asio::io_context::work>;
	using ThreadPtr = std::shared_ptr<std::thread>;

	explicit IOContextPool(uint16_t pool_size);

	void Run();
	void Join();
	void Stop();
	asio::io_context& get_io_context();

private:
	std::vector<IOCPtr> io_contexts_;
	std::vector<WorkPtr> works_;            //io_service::work���Ա�֤io_service��runû���¼�ִ�е������Ҳ�����˳�
	std::vector<ThreadPtr> threads_;

	std::mutex mutex_;
	uint16_t pool_size_;
	uint16_t next_io_context_;
};
