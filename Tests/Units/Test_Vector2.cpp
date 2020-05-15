#include <gtest/gtest.h>

#include <Maths/Vector2.hpp>

TEST(Vector2, operators) {
	acid::Vector2f a(3.0f, -7.2f);
	acid::Vector2f b(-1.74f, 15.4f);
	acid::Vector2ui c(5, 2);
	
	ASSERT_EQ(a + 20.0f, acid::Vector2f(23.0f, 12.8f));
	ASSERT_EQ(-1.11f * a, acid::Vector2f(-3.33f, 7.992f));
	ASSERT_EQ(a + b, acid::Vector2f(1.26f, 8.2f));
	ASSERT_EQ(a - b, acid::Vector2f(4.74f, -22.6f));
	ASSERT_EQ(a * b, acid::Vector2f(-5.22f, -110.88f));
	ASSERT_EQ(a / b, acid::Vector2f(-1.72414f, -0.467532f));
	ASSERT_EQ(a.Angle(b), 2.85931f);
	ASSERT_EQ(a.Dot(b), -116.1f);
	ASSERT_EQ(a.Lerp(b, 0.5f), acid::Vector2f(0.63f, 4.1f));
	ASSERT_EQ(a.Scale(10.0f), acid::Vector2f(30.0f, -72.0f));
	ASSERT_EQ(a.Rotate(1.5708f), acid::Vector2f(7.19999f, 3.00003f));
	ASSERT_EQ(-a, acid::Vector2f(-3.0f, 7.2f));
	ASSERT_EQ(a.Normalize(), acid::Vector2f(0.384615f, -0.923077f));
	ASSERT_EQ(a.Length(), 7.8f);
	ASSERT_EQ(a.Distance(b), 23.0917f);;
}
