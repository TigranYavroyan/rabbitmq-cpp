#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <dotenv.h>

std::string generate_ID () {
    std::string rand_ID = "123456789";

    std::random_device seed;
    std::mt19937 engine(seed());
    std::shuffle(rand_ID.begin(), rand_ID.end(), engine);

    return rand_ID;
}

int main() {
    dotenv::init();

    boost::asio::io_service ioService;
    AMQP::LibBoostAsioHandler handler(ioService);
    AMQP::Address address(std::getenv("AMQP_URL"));
    AMQP::TcpConnection connection(&handler, address);
    AMQP::TcpChannel channel(&connection);

    std::string replyQueue = std::getenv("REPLY_QUEUE");
    std::string workQueue = std::getenv("WORK_QUEUE");

    channel.declareQueue(workQueue);
    channel.declareQueue(replyQueue);

    channel.onError([](const char* msg){
        std::cout << "Channel error: " << msg << std::endl;
    });

    channel.onReady([](){
        std::cout << "The producer ready to send." << std::endl;
    });

    // Thread for stdin input
    std::thread inputThread([&]() {
        std::string line;

        while (std::getline(std::cin, line)) {

            // put callback in the event loop's queue
            boost::asio::post(ioService, [line, &channel, &replyQueue, &workQueue]() {
                AMQP::Envelope env(line);
                env.setReplyTo(replyQueue);

                env.setCorrelationID(generate_ID());

                // if no post here can be race condition
                channel.publish("", workQueue, env);
                std::cout << "Sent: " << line << std::endl;
            });
        }
    });

    // Consume replies
    channel.consume(replyQueue).onReceived([&channel, &ioService, &inputThread](const AMQP::Message &msg, uint64_t tag, bool redelivered) {
        std::string back_msg(msg.body(), msg.bodySize());

        std::cout << "Result: " << back_msg << std::endl;
        channel.ack(tag);

    });

    std::cout << "ASIO service start to run." << std::endl;
    ioService.run();
    inputThread.join();

    return 0;
}
