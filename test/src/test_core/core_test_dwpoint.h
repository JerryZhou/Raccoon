
/************************************************************************
	DwPoint
************************************************************************/

TEST(DwPoint, DwPoint)
{
	DwPoint p0;
	EXPECT_TRUE(p0.isNull());

	DwPoint p1(1, 1);
	EXPECT_FALSE(p1.isNull());
}

TEST(DwPoint, values)
{
	DwPoint p0(1, 2);
	EXPECT_EQ(1, p0.x());
	EXPECT_EQ(2, p0.y());
	EXPECT_EQ(3, p0.manhattanLength());

	DwPoint p1(2, 3);
	EXPECT_EQ(2, p1.rx());
	EXPECT_EQ(3, p1.ry());
	p1.rx() = 20;
	p1.ry() = 30;
	EXPECT_EQ(20, p1.x());
	EXPECT_EQ(30, p1.y());

	DwPoint p2;
	p2.setX(-1);
	p2.setY(2);
	EXPECT_EQ(-1, p2.x());
	EXPECT_EQ(2, p2.y());
}

TEST(DwPoint, operators)
{
	DwPoint p0(1, 2);
	DwPoint p1(2, 3);
	EXPECT_EQ(DwPoint(3, 5), p0 + p1);
	EXPECT_EQ(DwPoint(-1, -1), p0 - p1);
	EXPECT_EQ(DwPoint(3, 6), p0 * 3);
	EXPECT_EQ(DwPoint(8, 12), 4 * p1);
	EXPECT_EQ(DwPoint(2, 4), p0 / 0.5);

	EXPECT_EQ(DwPoint(-1, -2), - p0);

	DwPoint p2(3, 4);
	p2 += p0;
	EXPECT_EQ(4, p2.x());
	EXPECT_EQ(6, p2.y());

	p2 -= p0;
	EXPECT_EQ(3, p2.x());
	EXPECT_EQ(4, p2.y());

	p2 *= 2;
	EXPECT_EQ(6, p2.x());
	EXPECT_EQ(8, p2.y());

	p2 /= 2;
	EXPECT_EQ(3, p2.x());
	EXPECT_EQ(4, p2.y());
}

/************************************************************************
	DwPointF
************************************************************************/

TEST(DwPointF, DwPointF)
{
	DwPointF p0;
	EXPECT_TRUE(p0.isNull());

	DwPointF p1(1.0, 1.1);
	EXPECT_FALSE(p1.isNull());

    DwPoint p2(5, 6);
    DwPointF p3(p2);
    EXPECT_EQ(p3, DwPointF(5, 6));
}

TEST(DwPointF, values)
{
	DwPointF p0(1.1, 2.2);
	EXPECT_DOUBLE_EQ(1.1, p0.x());
	EXPECT_DOUBLE_EQ(2.2, p0.y());
	EXPECT_DOUBLE_EQ(3.3, p0.manhattanLength());

	DwPointF p1(2.1, 3.2);
	EXPECT_DOUBLE_EQ(2.1, p1.rx());
	EXPECT_DOUBLE_EQ(3.2, p1.ry());
	p1.rx() = 4.3;
	p1.ry() = 5.4;
	EXPECT_DOUBLE_EQ(4.3, p1.x());
	EXPECT_DOUBLE_EQ(5.4, p1.y());

	DwPointF p2;
	p2.setX(-1.1);
	p2.setY(2.2);
	EXPECT_DOUBLE_EQ(-1.1, p2.x());
	EXPECT_DOUBLE_EQ(2.2, p2.y());
}

TEST(DwPointF, operators)
{
	DwPointF p0(1.1, 2.2);
	DwPointF p1(2.2, 3.3);
	EXPECT_EQ(DwPointF(3.3, 5.5), p0 + p1);
	EXPECT_EQ(DwPointF(-1.1, -1.1), p0 - p1);
	EXPECT_EQ(DwPointF(3.3, 6.6), p0 * 3);
	EXPECT_EQ(DwPointF(8.8, 13.2), 4 * p1);
	EXPECT_EQ(DwPointF(2.2, 4.4), p0 / 0.5);

	EXPECT_EQ(DwPointF(-2.2, -3.3), - p1);

	DwPointF p2(3.3, 4.4);
	p2 += p0;
	EXPECT_DOUBLE_EQ(4.4, p2.x());
	EXPECT_DOUBLE_EQ(6.6, p2.y());

	p2 -= p0;
	EXPECT_DOUBLE_EQ(3.3, p2.x());
	EXPECT_DOUBLE_EQ(4.4, p2.y());

	p2 *= 2;
	EXPECT_DOUBLE_EQ(6.6, p2.x());
	EXPECT_DOUBLE_EQ(8.8, p2.y());

	p2 /= 2;
	EXPECT_DOUBLE_EQ(3.3, p2.x());
	EXPECT_DOUBLE_EQ(4.4, p2.y());
}

TEST(DwPointF, toPoint)
{
	DwPointF p0(1.1, 2.2);
	EXPECT_EQ(DwPoint(1, 2), p0.toPoint());

	DwPointF p1(5.7, 2.2);
	EXPECT_EQ(DwPoint(6, 2), p1.toPoint());

	DwPointF p2(1.1, 6.6);
	EXPECT_EQ(DwPoint(1, 7), p2.toPoint());

	DwPointF p3(1.9, 2.9);
	EXPECT_EQ(DwPoint(2, 3), p3.toPoint());
}