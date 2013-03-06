
/************************************************************************
	DwSize
************************************************************************/

TEST(DwSize, DwSize)
{
	DwSize s0;
	EXPECT_TRUE(s0.isNull());
	EXPECT_TRUE(s0.isEmpty());
	EXPECT_TRUE(s0.isValid());

	DwSize s1(1, 1);
	EXPECT_FALSE(s1.isNull());
	EXPECT_FALSE(s1.isEmpty());
	EXPECT_TRUE(s1.isValid());

	DwSize s2(-1, 1);
	EXPECT_FALSE(s2.isNull());
	EXPECT_TRUE(s2.isEmpty());
	EXPECT_FALSE(s2.isValid());

	DwSize s3(-1, -1);
	EXPECT_FALSE(s3.isNull());
	EXPECT_TRUE(s3.isEmpty());
	EXPECT_FALSE(s3.isValid());

	DwSize s4(1, -1);
	EXPECT_FALSE(s4.isNull());
	EXPECT_TRUE(s4.isEmpty());
	EXPECT_FALSE(s4.isValid());
}

TEST(DwSize, values)
{
	DwSize s0(1, 2);
	EXPECT_EQ(1, s0.width());
	EXPECT_EQ(2, s0.height());

	DwSize s1(2, 3);
	EXPECT_EQ(2, s1.rwidth());
	EXPECT_EQ(3, s1.rheight());
	s1.rwidth() = 10;
	s1.rheight() = 20;
	EXPECT_EQ(10, s1.width());
	EXPECT_EQ(20, s1.height());

	DwSize s2;
	s2.setWidth(20);
	s2.setHeight(30);
	EXPECT_EQ(20, s2.width());
	EXPECT_EQ(30, s2.height());

	s2.rwidth() += 20;
	s2.rheight() += 30;
	EXPECT_EQ(40, s2.width());
	EXPECT_EQ(60, s2.height());
}

TEST(DwSize, operators)
{
	DwSize s0(1, 2);
	DwSize s1(2, 3);
	EXPECT_EQ(DwSize(3, 5), s0 + s1);
	EXPECT_EQ(DwSize(-1, -1), s0 - s1);
	EXPECT_EQ(DwSize(3, 6), s0 * 3);
	EXPECT_EQ(DwSize(8, 12), 4 * s1);
	EXPECT_EQ(DwSize(2, 4), s0 / 0.5);

	DwSize s2(3, 4);
	s2 += s0;
	EXPECT_EQ(4, s2.width());
	EXPECT_EQ(6, s2.height());

	s2 -= s0;
	EXPECT_EQ(3, s2.width());
	EXPECT_EQ(4, s2.height());

	s2 *= 2;
	EXPECT_EQ(6, s2.width());
	EXPECT_EQ(8, s2.height());

	s2 /= 2;
	EXPECT_EQ(3, s2.width());
	EXPECT_EQ(4, s2.height());
}

TEST(DwSize, others)
{
	DwSize s0(2, 4);
	DwSize s1(4, 8);
	DwSize s2(10, 20);
	DwSize s3(20, 10);

	EXPECT_EQ(DwSize(2, 4), s0.boundedTo(s1));
	EXPECT_EQ(DwSize(4, 8), s0.expandedTo(s1));

	s0.scale(DwSize(10, 10), Dw::IgnoreAspectRatio);
	EXPECT_EQ(DwSize(10, 10), s0);

	s1.scale(DwSize(20, 20), Dw::KeepAspectRatio);
	EXPECT_EQ(DwSize(10, 20), s1);

	s2.scale(DwSize(100, 100), Dw::KeepAspectRatioByExpanding);
	EXPECT_EQ(DwSize(100, 200), s2);

	s3.scale(100, 100, Dw::KeepAspectRatioByExpanding);
	EXPECT_EQ(DwSize(200, 100), s3);

	s3.transpose();
	EXPECT_EQ(DwSize(100, 200), s3);

	DwSize s4(10, 5);
	s4.scale(20, 20, Dw::KeepAspectRatio);
	EXPECT_EQ(DwSize(20, 10), s4);
}

/************************************************************************
	DwSizeF
************************************************************************/

TEST(DwSizeF, DwSizeF)
{
	DwSizeF s0;
	EXPECT_TRUE(s0.isNull());
	EXPECT_TRUE(s0.isEmpty());
	EXPECT_TRUE(s0.isValid());

	DwSizeF s1(1.1, .11);
	EXPECT_FALSE(s1.isNull());
	EXPECT_FALSE(s1.isEmpty());
	EXPECT_TRUE(s1.isValid());

	DwSizeF s2(-1.1, 1.1);
	EXPECT_FALSE(s2.isNull());
	EXPECT_TRUE(s2.isEmpty());
	EXPECT_FALSE(s2.isValid());

	DwSizeF s3(-1.1, -1.1);
	EXPECT_FALSE(s3.isNull());
	EXPECT_TRUE(s3.isEmpty());
	EXPECT_FALSE(s3.isValid());

	DwSizeF s4(1.1, -1.1);
	EXPECT_FALSE(s4.isNull());
	EXPECT_TRUE(s4.isEmpty());
	EXPECT_FALSE(s4.isValid());

	DwSizeF s5(s4);
	EXPECT_EQ(s5, s4);

    DwSize s6(5, 6);
    DwSizeF s7(s6);
    EXPECT_EQ(s7, DwSizeF(5, 6));
}

TEST(DwSizeF, values)
{
	DwSizeF s0(1.1, 2.2);
	EXPECT_DOUBLE_EQ(1.1, s0.width());
	EXPECT_DOUBLE_EQ(2.2, s0.height());

	DwSizeF s1(2.2, 3.3);
	EXPECT_DOUBLE_EQ(2.2, s1.rwidth());
	EXPECT_DOUBLE_EQ(3.3, s1.rheight());
	s1.rwidth() = 10.1;
	s1.rheight() = 20.2;
	EXPECT_DOUBLE_EQ(10.1, s1.width());
	EXPECT_DOUBLE_EQ(20.2, s1.height());

	DwSizeF s2;
	s2.setWidth(20.2);
	s2.setHeight(30.3);
	EXPECT_DOUBLE_EQ(20.2, s2.width());
	EXPECT_DOUBLE_EQ(30.3, s2.height());

	s2.rwidth() += 20;
	s2.rheight() += 30;
	EXPECT_DOUBLE_EQ(40.2, s2.width());
	EXPECT_DOUBLE_EQ(60.3, s2.height());
}

TEST(DwSizeF, operators)
{
	DwSizeF s0(1.1, 2.2);
	DwSizeF s1(2.2, 3.3);
	EXPECT_EQ(DwSizeF(3.3, 5.5), s0 + s1);
	EXPECT_EQ(DwSizeF(-1.1, -1.1), s0 - s1);
	EXPECT_EQ(DwSizeF(3.3, 6.6), s0 * 3);
	EXPECT_EQ(DwSizeF(8.8, 13.2), 4 * s1);
	EXPECT_EQ(DwSizeF(2.2, 4.4), s0 / 0.5);

	DwSizeF s2(3.3, 4.4);
	s2 += s0;
	EXPECT_EQ(DwSizeF(4.4, 6.6), s2);

	s2 -= s0;
	EXPECT_EQ(DwSizeF(3.3, 4.4), s2);

	s2 *= 2;
	EXPECT_EQ(DwSizeF(6.6, 8.8), s2);

	s2 /= 2;
	EXPECT_EQ(DwSizeF(3.3, 4.4), s2);
}

TEST(DwSizeF, others)
{
	DwSizeF s0(2.2, 4.4);
	DwSizeF s1(4.4, 8.8);
	DwSizeF s2(10.1, 20.2);
	DwSizeF s3(20.2, 10.1);

	EXPECT_EQ(DwSizeF(2.2, 4.4), s0.boundedTo(s1));
	EXPECT_EQ(DwSizeF(4.4, 8.8), s0.expandedTo(s1));

	s0.scale(DwSizeF(10, 10), Dw::IgnoreAspectRatio);
	EXPECT_EQ(DwSizeF(10, 10), s0);

	s1.scale(DwSizeF(20, 20), Dw::KeepAspectRatio);
	EXPECT_EQ(DwSizeF(10, 20), s1);

	s2.scale(DwSizeF(100, 100), Dw::KeepAspectRatioByExpanding);
	EXPECT_EQ(DwSizeF(100, 200), s2);

	s3.scale(100, 100, Dw::KeepAspectRatioByExpanding);
	EXPECT_EQ(DwSizeF(200, 100), s3);

	s3.transpose();
	EXPECT_EQ(DwSizeF(100, 200), s3);

	DwSizeF s4(5.4, 6.6);
	EXPECT_EQ(DwSize(5, 7), s4.toSize());

	DwSizeF s5(10, 5);
	s5.scale(DwSizeF(20, 20), Dw::KeepAspectRatio);
	EXPECT_EQ(DwSizeF(20, 10), s5);
}