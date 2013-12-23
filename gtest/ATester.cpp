#include <gtest/gtest.h>
#include "HeadFile.h"

TEST(CAServerInit,Normal)
{
	CLLogger::Create();

	EXPECT_EQ(CLSqlite::Create(),0);
}