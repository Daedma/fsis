#include "NetworkServer.hpp"

NetworkServer::NetworkServer()
{
	//listener.setBlocking(true);
	

}

Socket::Status NetworkServer::init()
{
	if (listener.listen(Socket::AnyPort) == Socket::Status::Done)
	{
		cout << "Port -" << listener.getLocalPort() << endl;
		return Socket::Status();
	}
	else return Socket::Status::Error;
}

Socket::Status NetworkServer::registerNewClients()
{
	acceptIncomingConnection();
	receiveClientRegData();
	sendNewClientDataToAll();
	sendDedicatedDataPort();
	if (regStep == 5) { regStep = 0; return Socket::Status::Done; }
	else return Socket::Status::NotReady;
}

Socket::Status NetworkServer::acceptIncomingConnection()
{
	if (regStep == 0)
	{
		if (listener.isBlocking()) listener.setBlocking(false);
		

		//cout << "Port -" << listener.getLocalPort() << endl;
		if (listener.accept(regSocket) == Socket::Status::Done)
		{
			cout << "acceptIncomingConnection(): Accepted new connection\n";
			regStep = 1;
			return Socket::Status::Done;
		}
		else return Socket::Status::NotReady;

		return Socket::Status::NotReady;
	}
	return Socket::Status::Done;

}
bool checkLoginFormat(const std::string &login)
{
    // Проверка длины логина (от 4 до 20 символов)
    if (login.length() < 1 || login.length() > 18)
    {
        return false;
    }
    // Проверка символов логина (допустимы только буквы, цифры и символы '_', '-' и '.')
    for (char c : login)
    {
        if (!isalnum(c) && c != '_' && c != '-' && c != '.')
        {
            return false;
        }
    }
    return true;
}

// проверка пароля
bool checkPasswordFormat(const std::string &password)
{
    // Проверка длины логина (от 4 до 20 символов)
    if (password.length() < 1 || password.length() > 18)
    {
        return false;
    }
    // Проверка символов логина (допустимы только буквы, цифры и символы '_', '-' и '.')
    for (char c : password)
    {
        if (!isalnum(c) && c != '_' && c != '-' && c != '.')
        {
            return false;
        }
    }
    return true;
}

// Проверка наличия пользователя в БД
pqxx::connection conn("dbname=test user=postgres password=123454321 host=localhost port=5432");
pqxx::work txn(conn);

int authorization(const std::string &login, const std::string &password, Client *data)
{
    // Проверка формата логина и пароля
    if (!checkLoginFormat(login) || !checkPasswordFormat(password))
    {
        // Код ошибки -1: Некорректный формат логина или пароля
        std::cout << "Error";
        return 1;
    }

    pqxx::result r = txn.exec("SELECT * FROM users WHERE user_login = " + txn.quote(login) + " AND user_password = " + txn.quote(password));
    
    if (r.empty())
    {
        std::cout << "Database empty" << std::endl;
        return 1;
    }
    // Получение данных об аккаунте
    const pqxx::row row = r[0];
    data->login = row["user_login"].as<std::string>();
    data->user_id = row["user_id"].as<int>();
    data->password = row["user_password"].as<std::string>();
    return 0;
}



Socket::Status NetworkServer::receiveClientRegData()
{
	if (regStep == 1)
	{
		if (regSocket.isBlocking()) regSocket.setBlocking(false);

		if (regSocket.receive(packet) == Socket::Status::Done)
		{
			if (packet.getDataSize() > 0)
			{
				string login;
				string password;
				if (packet >> login >> password)
				{

					Client newClient;
					if(authorization(login, password, &data)==0){
						std::cout << "GOOD" << std::endl;
						clientsVec.push_back(newClient);
						clientsVec.back().user_id = data.user_id;
						clientsVec.back().login = data.login;
						clientsVec.back().password= data.password;
						clientsVec.back().Ip = regSocket.getRemoteAddress();
						clientsVec.back().dataSocket = new UdpSocket;
						if (clientsVec.back().dataSocket->bind(Socket::AnyPort) != Socket::Status::Done)
							cout << "(!)receiveClientRegData(): Failed to bind port to the new client-dedicated data port\n";
					}
					else{
						regStep = 2;
						return Socket::Status::Error;
					}
				}
				else
				{
					cout << "(!)receiveClientRegData(): Failed to read client name from received packet\n";
					return Socket::Status::Error;
				}

				Uint16 port;
				if (packet >> port)
				{
					clientsVec.back().port = static_cast<unsigned short>(port);
				}
				else
				{
					cout << "(!)receiveClientRegData(): Failed to read client data socket port from received packet\n";
					return Socket::Status::Error;
				}

				if (!packet.endOfPacket())
					cout << "(!)receiveClientRegData(): Client registration data received, but something left, data probably corrupted\n";
			}
			else
			{
				cout << "(!)receiveClientRegData(): Error, received packet is empty\n";
				return Socket::Status::Error;
			}

			cout << "receiveClientRegData(): Client registration data received. New client: " << clientsVec.back().login<< endl;
			cout << "receiveClientRegData(): Client registration data received. ID " << clientsVec.back().user_id<< endl;
			regStep = 2;
			for (int i = 0; i < clientsVec.size() - 1; i++)
				clientsVec[i].done = false;
			packet.clear();
			return Socket::Status::Done;
		}
		else return Socket::Status::NotReady;
	}
	return Socket::Status::Done;

}

Socket::Status NetworkServer::sendNewClientDataToAll()
{
	if (regStep == 2)
	{
		if (clientsVec.size() > 1)
		{
			for (int i = 0; i < clientsVec.size() - 1; i++)
			{
				if (!clientsVec[i].done)
				{
					if (clientsVec[i].dataSocket->isBlocking()) clientsVec[i].dataSocket->setBlocking(false);
					IpAddress tempIp = clientsVec[i].Ip;
					unsigned short tempPort = clientsVec[i].port;

					if (clientsVec[i].sDataPacket.getDataSize() == 0)
						clientsVec[i].sDataPacket << "NEW" << clientsVec.back().login;

					if (clientsVec[i].dataSocket->send(clientsVec[i].sDataPacket, tempIp, tempPort) == Socket::Status::Done)
					{
						clientsVec[i].done = true;

						bool allIsDone = true;
						for (int k = 0; k < clientsVec.size() - 1; k++)
						{
							if (!clientsVec[k].done) allIsDone = false;
						}
						if (allIsDone)
						{
							for (int j = 0; j < clientsVec.size(); j++)
							{
								clientsVec[j].sDataPacket.clear();
								clientsVec[j].done = false;
							}
							regStep = 3;
							return Socket::Status::Done;
						}
					}
				}
			}
			return Socket::Status::NotReady;
		}
		else
		{
			regStep = 3;
			return Socket::Status::Done;
		}
	}
	return Socket::Status::Done;
}

Socket::Status NetworkServer::sendDedicatedDataPort()
{
	if (regStep == 3)
	{
		if (regSocket.isBlocking()) regSocket.setBlocking(false);

		if (packet.getDataSize() == 0)
			packet << static_cast<Uint16>(clientsVec.back().dataSocket->getLocalPort());

		if (regSocket.send(packet) == Socket::Status::Done)
		{
			cout << "sendDedicatedDataPort(): Dedicated data port sent\n";
			regStep = 4;
			packet.clear();
			return Socket::Status::Done;
		}
		else return Socket::Status::NotReady;
	}
	return Socket::Status::Done;
}

Socket::Status NetworkServer::sendConnectedClientsRecords()
{
	if (regStep == 4)
	{
		if (regSocket.isBlocking()) regSocket.setBlocking(false);

		if (packet.getDataSize() == 0)
		{
			if (clientsVec.size() > 1)
			{
				for (int i = 0; i < clientsVec.size() - 1; i++){
					packet << clientsVec[i].login;
				}
			}

			else packet << "FIRST";
		}

		if (regSocket.send(packet) == Socket::Status::Done)
		{
			cout << "sendConnectedClientsRecords(): Connected clients records sent to new client\n";
			for (int i = 0; i < clientsVec.size(); i++){
					cout << clientsVec[i].login;
				}
			regStep = 5;
			regSocket.disconnect();
			return Socket::Status::Done;
		}
		else return Socket::Status::NotReady;
	}
	return Socket::Status::Done;
}

Socket::Status NetworkServer::receiveData(unsigned int& receivedClientIndex)
{
	for (int i = 0; i < clientsVec.size(); i++)
	{
		if (clientsVec[i].dataSocket->isBlocking()) clientsVec[i].dataSocket->setBlocking(false);
		IpAddress tempIp = clientsVec[i].Ip;
		unsigned short tempPort = clientsVec[i].port;

		if (clientsVec[i].dataSocket->receive(clientsVec[i].rDataPacket, tempIp, tempPort) == Socket::Status::Done)
		{
			receivedClientIndex = i;
			
			return Socket::Status::Done;
		}
	}

	return Socket::Status::NotReady;
}

Socket::Status NetworkServer::sendDataToAll(Packet dataPacket)
{
	if (sendingsRateTimer.getElapsedTime().asMilliseconds() > sendingsRate)
	{
		for (int i = 0; i < clientsVec.size(); i++)
		{
			if (!clientsVec[i].done)
			{
				if (clientsVec[i].dataSocket->isBlocking()) clientsVec[i].dataSocket->setBlocking(false);
				IpAddress tempIp = clientsVec[i].Ip;
				unsigned short tempPort = clientsVec[i].port;

				if (clientsVec[i].sDataPacket.getDataSize() == 0) clientsVec[i].sDataPacket = dataPacket;

				if (clientsVec[i].dataSocket->send(clientsVec[i].sDataPacket, tempIp, tempPort) == Socket::Status::Done)
				{
					clientsVec[i].done = true;
				}

				bool allIsDone = true;

				for (int j = 0; j < clientsVec.size(); j++)
				{
					if (clientsVec[j].done == false) allIsDone = false;
				}

				if (allIsDone)
				{
					for (int j = 0; j < clientsVec.size(); j++)
					{
						clientsVec[j].sDataPacket.clear();
						clientsVec[j].done = false;
					}
					sendingsRateTimer.restart();
					return Socket::Status::Done;
				}
			}

		}
		return Socket::Status::NotReady;

	}
	else return Socket::Status::NotReady;
}















/*

class MyAdapter : public Adapter
{
public:
    virtual void GetAddress( Address & address )
    {
        address.Parse( "127.0.0.1:40000" );
    }
};

class MyServer : public Server
{
public:
    MyServer( Allocator & allocator, const uint8_t * privateKey ) : Server( allocator, privateKey ) {}

    virtual void OnClientConnect( int clientIndex )
    {
        printf( "client %d connected\n", clientIndex );
    }

    virtual void OnClientDisconnect( int clientIndex )
    {
        printf( "client %d disconnected\n", clientIndex );
    }

    virtual void OnPacketReceived( int clientIndex, const uint8_t * packetData, int packetSize )
    {
        printf( "server received packet from client %d\n", clientIndex );
        
        // Декодируем данные пакета с использованием описанного ранее формата.
        // Получаем логин и пароль от клиента.
        bool userExists = false;
        uint8_t login[MAX_LOGIN_LENGTH];
        uint8_t password[MAX_PASSWORD_LENGTH];
        int loginLength = 0, passwordLength = 0;
        DecodePacket( packetData, packetSize, login, loginLength, password, passwordLength );
        
        // Обрабатываем логин и пароль.
        // Проверяем наличие пользователя в базе данных.
        if (strcmp((const char*)login, "user1") == 0 && strcmp((const char*)password, "password1") == 0)
        {
            userExists = true;
        }
        
        // Отправляем ответ клиенту.
        uint8_t response = userExists ? 1 : 0;
        uint8_t responseData[1];
        int responseSize = yojimbo::EncodePacket( userExists, response, responseData, sizeof(responseData) );
        SendPacket( clientIndex, responseData, responseSize );
    }
};

int main()
{
    // Создаем адаптер и сервер.
    MyAdapter adapter;
    MyServer server( GetDefaultAllocator(), GetRandomBytes( YOJIMBO_KEY_BYTES ) );

    // Добавляем канал связи.
    int channelId = server.CreateChannel();
    assert( channelId == 0 );

    // Запускаем сервер.
    server.Start( 1 );
    
    return 0;
}
*/