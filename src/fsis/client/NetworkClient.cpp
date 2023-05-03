#include "yojimbo.h"
#include <iostream>
#include <SFML/Network.hpp>
#include "NetworkClient.hpp"

NetworkClient::NetworkClient()
{

}
//инициализация сокета
sf::Socket::Status NetworkClient::init(unsigned short preferablePort) 
{
	sf::Socket::Status status = dataSocket.bind(preferablePort); //sf::Socket::AnyPort чтобы забиндить на любой свободной порт

	if (status == sf::Socket::Status::Done) //Done - отправил или получил данные
	{
		std::cout << "Successfully binded to port: " << dataSocket.getLocalPort() << std::endl; //выводим порт
		return sf::Socket::Status::Done;
	}
	else
	{
		std::cout << "(!)init(): Failed to bind passed preferred port\n";
		do
		{
			unsigned short newPort = sf::Socket::AnyPort;
			std::cout <<  "Trying to bind other port - " << newPort << std::endl;
			status = dataSocket.bind(newPort);
			if (status != sf::Socket::Status::Done)
				std::cout << "Failed to bind other port. Retrying...\n";

		} while (status != sf::Socket::Done);

		std::cout <<  "Successfully binded to other port - " << dataSocket.getLocalPort() << std::endl;
		return sf::Socket::Status::Done;
	}
}
//метод для получения списка подключённых пользователей
sf::Socket::Status NetworkClient::receiveConnectedClientsNames(std::vector<std::string>& namesVec)
{
	if (!regSocket.isBlocking()) regSocket.setBlocking(true);

	sf::Packet tempPacket;

	if (regSocket.receive(tempPacket) == sf::Socket::Status::Done)
	{
		if (tempPacket.getDataSize() > 0)
		{
			while (!tempPacket.endOfPacket())
			{
				std::string name;
				if (tempPacket >> name)
				{
					if (name == "FIRST")
					{
						std::cout << "receiveConnectedClientsNames(): No clients are connected, you are first\n";
						return sf::Socket::Status::Done;
					}
					namesVec.push_back(name);
				}
				else
				{
					std::cout << "(!)receiveConnectedClientsNames() : Failed to read packet\n";
					return sf::Socket::Status::Error;
				}
			}
			std::cout << "receiveConnectedClientsNames() :Client names read\n";

			for (int i = 0; i < namesVec.size(); i++)
			{
				std::cout << namesVec[i];
			}
			std::cout << std::endl;
			return sf::Socket::Status::Done;

		}
		else std::cout << "(!)receiveConnectedClientsNames(): Receives packet is empty, ensure that packet contains: (string name1 << string name2 << ...) or \"FIRST\" if it's first connected client\n";
	}
	else std::cout << "(!)receiveConnectedClientsNames(): Failed to receive clients names\n";

	return sf::Socket::Status::Error;
}
//получение данных
sf::Socket::Status NetworkClient::receiveData(sf::Packet& dataPacket, sf::IpAddress S_Ip, unsigned short S_dataPort)
{
	if (dataSocket.isBlocking())dataSocket.setBlocking(false);

	if (dataSocket.receive(dataPacket, S_Ip, S_dataPort) == sf::Socket::Status::Done)
	{
		if (dataPacket.getDataSize() > 0)
		{
			//cout << "receiveData(): Data received\n";
			return sf::Socket::Status::Done;
		}
		else
		{
			std::cout << "(!)receiveData(): Received packet is empty\n";
			return sf::Socket::Status::Error;
		}
	}
	return sf::Socket::Status::NotReady;
}
//отправка данных
sf::Socket::Status NetworkClient::sendData(sf::Packet dataPacket)
{
	if (sendRateTimer.getElapsedTime().asMilliseconds() > sendRate)
	{
		if (dataSocket.isBlocking())dataSocket.setBlocking(false);
		
		if (sendPacket.getDataSize() == 0) sendPacket = dataPacket;

		sf::IpAddress tempIp = S_Ip;
		unsigned short tempDataPort = S_dataPort;
		if (dataSocket.send(sendPacket, tempIp, tempDataPort) == sf::Socket::Status::Done)
		{
			sendPacket.clear();
			sendRateTimer.restart();
			return sf::Socket::Status::Done;
		}
		else return sf::Socket::Status::NotReady;
	}
	else return sf::Socket::Status::NotReady;
}

void NetworkClient::setSendFreq(sf::Int32 milliseconds)
{
	sendRate = milliseconds;
}
//регистрация на сервере
sf::Socket::Status NetworkClient::registerOnServer(sf::IpAddress serverIp, unsigned short serverRegPort, std::string clientName, std::string password)
{
	if (connectRegTcpSocket(serverIp, serverRegPort) != sf::Socket::Status::Done){
	
		return sf::Socket::Status::Error;
		
	}

	if (sendClientRecipientData(clientName, password) != sf::Socket::Status::Done){
		
		return sf::Socket::Status::Error;
		
	}

	if  (recieveDedicatedDataServerPort() != sf::Socket::Status::Done){
		
		return sf::Socket::Status::Error;
		
	}
	return sf::Socket::Status::Done;
	
}
//подключение к серверу
sf::Socket::Status NetworkClient::connectRegTcpSocket(sf::IpAddress serverIp, unsigned short serverRegPort)
{
	if (!regSocket.isBlocking()) regSocket.setBlocking(true);

	if (regSocket.connect(serverIp, serverRegPort, sf::seconds(10)) == sf::Socket::Status::Done)
	{
		std::cout << " Connected to server\n";
		S_Ip = serverIp;
		S_dataPort = serverRegPort;
		return sf::Socket::Status::Done;
	}
	else
	{
		std::cout << " Error connecting to server!\n";
		return sf::Socket::Status::Error;
	}
}
//отправка данных
sf::Socket::Status NetworkClient::sendClientRecipientData(std::string clientName,  std::string password)
{
	if (!regSocket.isBlocking()) regSocket.setBlocking(true);

	sf::Packet tempPacket;
	tempPacket << clientName << password << static_cast<sf::Uint16>(dataSocket.getLocalPort());

	if (regSocket.send(tempPacket) == sf::Socket::Status::Done)
	{
		std::cout << " Successfully sent client recipient data\n";
		return sf::Socket::Status::Done;
	}
	else
	{
		std::cout << " Failed to send client recipient data\n";
		return sf::Socket::Status::Error;
	}
}
//получение данных о сервере
sf::Socket::Status NetworkClient::recieveDedicatedDataServerPort()
{
	if (!regSocket.isBlocking()) regSocket.setBlocking(true);

	sf::Packet tempPacket;

	if (regSocket.receive(tempPacket) == sf::Socket::Status::Done)
	{
		if (tempPacket.getDataSize() > 0)
		{
			if (tempPacket.getDataSize() == sizeof(sf::Uint16))
			{
				if (tempPacket >> S_dataPort)
				{
					std::cout << "recieveDedicatedDataServerPort(): Successfully received data client-dedicated port of a server - " << S_dataPort << std::endl;
					return sf::Socket::Status::Done;
				}
				else std::cout  << "(!)recieveDedicatedDataServerPort(): Failed to read from received packet\n";
			}
			else std::cout  << "(!)recieveDedicatedDataServerPort(): Invalid packet size, ensure that server sends only Uint16 var\n";
		}
		else std::cout  << "(!)recieveDedicatedDataServerPort(): Received packet is empty\n";
	}
	else std::cout  << "(!)recieveDedicatedDataServerPort(): Failed to receive client-dedicated port of a server\n";

	return sf::Socket::Status::Error;
}




/*

using namespace yojimbo;

class MyAdapter : public Adapter
{
public:
    virtual void GetAddress( Address & address )
    {
        address.Parse( "127.0.0.1:40000" );
    }
};

class MyClient : public Client
{
public:
    MyClient( Allocator & allocator, const uint8_t * privateKey ) : Client( allocator, privateKey ) {}

    virtual void OnConnect()
    {
        printf( "client connected\n" );
        
        // Кодируем логин и пароль в соответствии с описанным ранее форматом.
        // Отправляем пакет серверу.
        uint8_t login[] = "user1";
        uint8_t password[] = "password1";
        uint8_t packetData[1024];
        int packetSize = EncodePacket( login, sizeof(login), password, sizeof(password), packetData, sizeof(packetData) );
        SendPacket( packetData, packetSize );
    }

    virtual void OnDisconnect()
    {
        printf( "client disconnected\n" );
    }

    virtual void OnPacketReceived( const Address & sender, const uint8_t * packetData, int packetSize )
    {
        printf( "client received packet from server\n" );
        
        // Декодируем данные пакета с использованием описанного ранее формата.
        // Получаем ответ сервера.
        bool userExists = false;
        uint8_t response = 0;
        DecodePacket( packetData, packetSize, userExists, response );
        
        // Обрабатываем ответ сервера.
        if (userExists)
        {
            printf( "User found in the database\n" );
        }
        else
        {
            printf( "User not found in the database\n" );
        }
    }
};

int main()
{
    // Создаем адаптер и клиент.
    MyAdapter adapter;
    MyClient client( GetDefaultAllocator(), GetRandomBytes( YOJIMBO_KEY_BYTES ) );

    // Запускаем клиент.
    client.Start( 0 );

    // Подключаемся к серверу.
    client.Connect();

    // Основной цикл клиента.
    while ( true )
    {
        client.SendPackets();
        client.ReceivePackets();
        client.AdvanceTime( 0.1 );
    }

    // Останавливаем клиент.
    client.Disconnect();
    client.Stop();
}
*/