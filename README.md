# AMQP-CPP Producer-Consumer Example

This project demonstrates a **bidirectional communication system** using [AMQP-CPP](https://github.com/CopernicaMarketingSoftware/AMQP-CPP) and **Boost.Asio** in C++.  
A **producer** sends commands to be executed, and a **consumer** executes the commands and sends back the result.

---

## 📦 Features

✅ Sends shell commands from the producer to the consumer  
✅ Consumer executes the command and replies with the output  
✅ Uses `AMQP::LibBoostAsioHandler` for integration with Boost.Asio  
✅ Bidirectional messaging with `replyTo` and `correlationID`  

---

### Install dependencies

#### 🐇 RabbitMQ
You need RabbitMQ server running locally or remotely.

```bash
sudo apt update
sudo apt install rabbitmq-server
sudo systemctl start rabbitmq-server
```
⚡ Boost
Install Boost.Asio:

```bash
sudo apt install libboost-dev libboost-system-dev
```

#### 🔗 AMQP-CPP
Clone and build AMQP-CPP:

You can build the AMQP-CPP lib based on it's documentation
https://github.com/CopernicaMarketingSoftware/AMQP-CPP.git

Setup the .env file (change .env.example to .env)

For example:

```
AMQP_URL=amqp://guest:guest@localhost/
WORK_QUEUE=work_queue
REPLY_QUEUE=reply_queue
```

#### 🛠 Build
Just run this command:
```
make
```

🚀 Usage
Start RabbitMQ (if not already running):
```
make broker_run
```
Run the consumer:
```
make consumer_run
```
Run the producer (in another terminal):
```
make producer_run
```

#### Now you can just input in stdin some commands for execution in shell (pwd, ls, etc...) and get results of execution.

---
