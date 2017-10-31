//
// Created by Kévin POLOSSAT on 31/10/2017.
//

#include "socket.h"
#include "lw_tcp_server_exception.h"

#include "gtest/gtest.h"

class SocketTest: public ::testing::Test {
public:
    SocketTest() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(SocketTest, instantiation) {
    lw_tcp_server::Socket s;
    lw_tcp_server::Socket s2(42, 43, 44);

    ASSERT_EQ(s.Protocol(), -1);
    ASSERT_EQ(s.Type(), -1);
    ASSERT_EQ(s.Domain(), -1);
    ASSERT_EQ(s2.Domain(), 42);
    ASSERT_EQ(s2.Type(), 43);
    ASSERT_EQ(s2.Protocol(), 44);

    lw_tcp_server::Socket s3(std::move(s2));
    ASSERT_EQ(s3.Domain(), 42);
    ASSERT_EQ(s3.Type(), 43);
    ASSERT_EQ(s3.Protocol(), 44);

}

TEST_F(SocketTest, open) {
    lw_tcp_server::Socket s;
    lw_tcp_server::Socket s2(42, 43, 44);

    EXPECT_THROW(s.Open(), lw_tcp_server::OpenError);
    EXPECT_THROW(s2.Open(), lw_tcp_server::OpenError);
}

TEST_F(SocketTest, close) {
    lw_tcp_server::Socket s;
    lw_tcp_server::Socket s2(42, 43, 44);

    EXPECT_THROW(s.Close(), lw_tcp_server::CloseError);
    EXPECT_THROW(s2.Close(), lw_tcp_server::CloseError);
}