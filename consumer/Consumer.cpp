#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <memory>
#include <chrono>
#include <cstdlib>
#include <unistd.h>
#include <dotenv.h>

std::string execute_command(const std::string &cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "Failed to run command";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}

int main() { 
    dotenv::init();

    boost::asio::io_service ioService;
    AMQP::LibBoostAsioHandler handler(ioService);
    AMQP::Address address(std::getenv("AMQP_URL"));
    AMQP::TcpConnection connection(&handler, address);
    AMQP::TcpChannel channel(&connection);

    std::string workQueue = std::getenv("WORK_QUEUE");

    channel.declareQueue(workQueue);

    channel.onReady([](){
        std::cout << "The consumer ready to serve." << std::endl;
    });

    channel.onError([](const char* msg){
        std::cout << "Channel error: " << msg << std::endl;
    });

    ioService.dispatch([](){
        std::cout << "ASIO service start to run." << std::endl;
    });

    channel.consume(workQueue).onReceived([&channel, &ioService](const AMQP::Message &msg, uint64_t tag, bool redelivered) {

        std::string cmd(msg.body(), msg.bodySize());
        std::cout << "Executing: " << cmd << std::endl;

        std::string result = execute_command(cmd);

        if (!msg.replyTo().empty()) {
            AMQP::Envelope env(result);
            env.setCorrelationID(msg.correlationID());
            channel.publish("", msg.replyTo(), env);
            std::cout << "Result sent back.\n";
        }
        
        channel.ack(tag);
    });

    ioService.run();

    return 0;
}
