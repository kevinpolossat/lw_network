//
// Created by Kévin POLOSSAT on 31/10/2017.
//

#include "socket.h"

#include "gtest/gtest.h"

class SocketTest: public ::testing::Test {
public:
    SocketTest() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(SocketTest, instantiation) {
    lw_tcp_socket::Socket s;
    lw_tcp_socket::Socket s2(42, 43, 44);

    ASSERT_EQ(s.Protocol(), -1);
    ASSERT_EQ(s.Type(), -1);
    ASSERT_EQ(s.Domain(), -1);
    ASSERT_EQ(s2.Domain(), 42);
    ASSERT_EQ(s2.Type(), 43);
    ASSERT_EQ(s2.Protocol(), 44);
}