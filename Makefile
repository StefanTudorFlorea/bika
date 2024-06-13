up:
	docker-compose up -d
	cd frontend && echo "Bursuc.gp1" | sudo -S go run cmd/web/main.go
down:
	docker-compose down

build-broker:
	docker-compose build broker-service
build-auth:
	docker-compose build auth-service
build-logger:
	docker-compose build logger-service

build: build-broker build-auth build-logger