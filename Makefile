CONSUMER = consumer
PRODUCER = producer

SUBDIRS = $(PRODUCER) $(CONSUMER)

RED    = \033[1;31m
GREEN  = \033[1;32m
RESET  = \033[0;37m
BOLD_PURPLE = \033[1;35m
BOLD_SKY    = \033[1;36m

all: ## Build producer and consumer subprojects
	@for dir in $(SUBDIRS); do \
		echo "$(GREEN)[Top] Building $$dir...$(RESET)"; \
		$(MAKE) --no-print-directory -C $$dir; \
	done
	@echo "$(GREEN)[Top] The building is done$(RESET)"

clean: ## Remove build files in producer and consumer
	@for dir in $(SUBDIRS); do \
		echo "$(RED)[Top] Cleaning $$dir...$(RESET)"; \
		$(MAKE) --no-print-directory -C $$dir clean; \
	done

fclean: ## Full clean (including binaries) in producer and consumer
	@for dir in $(SUBDIRS); do \
		echo "$(RED)[Top] Full clean $$dir...$(RESET)"; \
		$(MAKE) --no-print-directory -C $$dir fclean; \
	done

re: fclean all ## Full rebuild

push: ## Git add, commit, and push (with message prompt)
	@read -p "Input the commit message: " msg; \
	git add .; \
	git commit -m "$$msg"; \
	git push

producer_run: ## Run the producer binary
	@$(PRODUCER)/producer

consumer_run: ## Run the consumer binary
	@$(CONSUMER)/consumer

broker_run: ## Start RabbitMQ broker using systemctl
	@systemctl start rabbitmq-server.service

broker_status: ## Show RabbitMQ broker status
	@systemctl status rabbitmq-server.service

help: ## Show this help message
	@echo "$(BOLD_PURPLE)Available targets:$(RESET)"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
	awk 'BEGIN {FS = ":.*?## "}; {printf "$(BOLD_SKY) - %-20s$(RESET) %s\n", $$1, $$2}'

.PHONY: all clean fclean re push producer_run consumer_run broker_run broker_status help
