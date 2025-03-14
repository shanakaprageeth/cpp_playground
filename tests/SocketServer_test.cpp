#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "socket/SocketServer.hpp"

// Mock class for SocketServer
class MockSocketServer : public SocketServer {
public:
    MOCK_METHOD(int, initSocket, (), (override));
    MOCK_METHOD(int, setSocket, (), (override));
    MOCK_METHOD(int, bindAndListen, (), (override));
    MOCK_METHOD(int, acceptClients, (), (override));
    MOCK_METHOD(int, readAndSend, (const char *rcvMsg, const char *sendMsg), (override));
};

TEST(SocketServerTest, Initialization) {
    MockSocketServer mockServer;
    EXPECT_CALL(mockServer, initSocket()).Times(1).WillOnce(::testing::Return(0));

    ASSERT_EQ(mockServer.initSocket(), 0);
}

TEST(SocketServerTest, Connection) {
    MockSocketServer mockServer;
    EXPECT_CALL(mockServer, initSocket()).Times(1).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockServer, acceptClients()).Times(1).WillOnce(::testing::Return(0));

    ASSERT_EQ(mockServer.initSocket(), 0);
    ASSERT_EQ(mockServer.acceptClients(), 0);
}

TEST(SocketServerTest, DataTransmission) {
    MockSocketServer mockServer;
    const char *dataToSend = "Hello, World!";
    const char *expectedResponse = "Hello, World!";

    EXPECT_CALL(mockServer, initSocket()).Times(1).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockServer, acceptClients()).Times(1).WillOnce(::testing::Return(0));
    EXPECT_CALL(mockServer, readAndSend(dataToSend, expectedResponse)).Times(1).WillOnce(::testing::Return(0));

    ASSERT_EQ(mockServer.initSocket(), 0);
    ASSERT_EQ(mockServer.acceptClients(), 0);
    ASSERT_EQ(mockServer.readAndSend(dataToSend, expectedResponse), 0);
}

