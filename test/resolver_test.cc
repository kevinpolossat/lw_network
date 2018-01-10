//
// Created by Kévin POLOSSAT on 01/11/2017.
//

#include "resolver.h"
#include "gtest/gtest.h"

class ResolverTest: public ::testing::Test {
public:
    ResolverTest() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(ResolverTest, instantiation) {
    lw_network::Resolver resolver("localhost", "4242", 42, 43, 44, 45);
    ASSERT_EQ(resolver.Node(), "localhost");
    ASSERT_EQ(resolver.Service(), "4242");
    ASSERT_EQ(resolver.Family(), 42);
    ASSERT_EQ(resolver.SockType(), 43);
    ASSERT_EQ(resolver.Protocol(), 44);
    ASSERT_EQ(resolver.Flags(), 45);
}

TEST_F(ResolverTest, getter_and_setter) {
    lw_network::Resolver resolver("localhost", "4242", 42, 43, 44, 45);

    resolver.SetNode("127.0.0.1");
    resolver.SetService("8484");
    resolver.SetFamily(1);
    resolver.SetSockType(2);
    resolver.SetProtocol(3);
    resolver.SetFlags(4);
    ASSERT_EQ(resolver.Node(), "127.0.0.1");
    ASSERT_EQ(resolver.Service(), "8484");
    ASSERT_EQ(resolver.Family(), 1);
    ASSERT_EQ(resolver.SockType(), 2);
    ASSERT_EQ(resolver.Protocol(), 3);
    ASSERT_EQ(resolver.Flags(), 4);
}