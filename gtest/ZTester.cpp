#include <gtest/gtest.h>

#include "HeadFile.h"

TEST(CAServerUninit,Normal)
{
	EXPECT_EQ(CLSqlite::Destroy(),0);
	CLLogger::Destroy();
}