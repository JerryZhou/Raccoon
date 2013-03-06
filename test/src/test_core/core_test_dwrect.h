
/************************************************************************
	DwRect
************************************************************************/

TEST(DwRect, DwRect)
{
	DwRect r0;
	EXPECT_TRUE(r0.isNull());
	EXPECT_TRUE(r0.isEmpty());
	EXPECT_FALSE(r0.isValid());

	DwRect r1(1, 1, 10, 10);
	EXPECT_FALSE(r1.isNull());
	EXPECT_FALSE(r1.isEmpty());
	EXPECT_TRUE(r1.isValid());

	DwRect r2(5, 5, -10, -10);
	EXPECT_FALSE(r2.isNull());
	EXPECT_TRUE(r2.isEmpty());
	EXPECT_FALSE(r2.isValid());

	DwRect r3(-1, -1, 10, 10);
	EXPECT_FALSE(r3.isNull());
	EXPECT_FALSE(r3.isEmpty());
	EXPECT_TRUE(r3.isValid());

	DwRect r4(1, -1, 10, -10);
	EXPECT_FALSE(r4.isNull());
	EXPECT_TRUE(r4.isEmpty());
	EXPECT_FALSE(r4.isValid());

	DwRect r5(DwPoint(3, 3), DwSize(10, 10));
	DwRect r6(DwPoint(3, 3), DwPoint(12, 12));
	EXPECT_EQ(r5, r6);

    DwRectF r7(r5);
    EXPECT_EQ(r7, DwRectF(3.0, 3.0, 10.0, 10.0));
}

TEST(DwRect, values)
{
	DwRect r0(1, 2, 10, 11);
	EXPECT_EQ(1, r0.x());
	EXPECT_EQ(2, r0.y());
	EXPECT_EQ(10, r0.width());
	EXPECT_EQ(11, r0.height());
	EXPECT_EQ(1, r0.left());
	EXPECT_EQ(10, r0.right());
	EXPECT_EQ(2, r0.top());
	EXPECT_EQ(12, r0.bottom());
	EXPECT_EQ(DwPoint(1, 12), r0.bottomLeft());
	EXPECT_EQ(DwPoint(10, 12), r0.bottomRight());
	EXPECT_EQ(DwPoint(1, 2), r0.topLeft());
	EXPECT_EQ(DwPoint(10, 2), r0.topRight());

	DwRect r1(DwPoint(2, 3), DwSize(12, 13));

	r1.setX(40);
	r1.setY(60);
	r1.setWidth(400);
	r1.setHeight(600);
	EXPECT_EQ(40, r1.x());
	EXPECT_EQ(60, r1.y());
	EXPECT_EQ(400, r1.width());
	EXPECT_EQ(600, r1.height());

	r1.moveLeft(30);
	EXPECT_EQ(DwPoint(30, 60), r1.topLeft());
	EXPECT_EQ(DwPoint(429, 659), r1.bottomRight());

	r1.moveRight(439);
	EXPECT_EQ(DwPoint(40, 60), r1.topLeft());
	EXPECT_EQ(DwPoint(439, 659), r1.bottomRight());

	r1.moveCenter(DwPoint(340, 460));
	EXPECT_EQ(DwPoint(141, 161), r1.topLeft());
	EXPECT_EQ(DwPoint(540, 760), r1.bottomRight());

	r1.moveTo(30, 50);
	EXPECT_EQ(DwPoint(30, 50), r1.topLeft());
	EXPECT_EQ(DwPoint(429, 649), r1.bottomRight());
	
	r1.moveTo(DwPoint(20, 40));
	EXPECT_EQ(DwPoint(20, 40), r1.topLeft());
	EXPECT_EQ(DwPoint(419, 639), r1.bottomRight());
	
	r1.moveTop(10);
	EXPECT_EQ(DwPoint(20, 10), r1.topLeft());
	EXPECT_EQ(DwPoint(419, 609), r1.bottomRight());

	r1.moveTopLeft(DwPoint(10, 10));
	EXPECT_EQ(DwPoint(10, 10), r1.topLeft());
	EXPECT_EQ(DwPoint(409, 609), r1.bottomRight());

	r1.moveTopRight(DwPoint(419, 20));
	EXPECT_EQ(DwPoint(20, 20), r1.topLeft());
	EXPECT_EQ(DwPoint(419, 619), r1.bottomRight());

	r1.moveBottom(619);
	EXPECT_EQ(DwPoint(20, 20), r1.topLeft());
	EXPECT_EQ(DwPoint(419, 619), r1.bottomRight());

	r1.moveBottomLeft(DwPoint(40, 639));
	EXPECT_EQ(DwPoint(40, 40), r1.topLeft());
	EXPECT_EQ(DwPoint(439, 639), r1.bottomRight());

	r1.moveBottomRight(DwPoint(449, 649));
	EXPECT_EQ(DwPoint(50, 50), r1.topLeft());
	EXPECT_EQ(DwPoint(449, 649), r1.bottomRight());

	EXPECT_EQ(DwSize(400, 600), r1.size());

	DwRect r2(10, 10, 100, 100);
	r2.setLeft(20);
	EXPECT_EQ(DwRect(20, 10, 90, 100), r2);
	r2.setTop(20);
	EXPECT_EQ(DwRect(20, 20, 90, 90), r2);
	r2.setRight(99);
	EXPECT_EQ(DwRect(20, 20, 80, 90), r2);
	r2.setBottom(99);
	EXPECT_EQ(DwRect(20, 20, 80, 80), r2);
	r2.setTopLeft(DwPoint(30, 30));
	EXPECT_EQ(DwRect(30, 30, 70, 70), r2);
	r2.setTopRight(DwPoint(89, 40));
	EXPECT_EQ(DwRect(30, 40, 60, 60), r2);
	r2.setBottomLeft(DwPoint(40, 79));
	EXPECT_EQ(DwRect(40, 40, 50, 40), r2);
	r2.setBottomRight(DwPoint(69, 69));
	EXPECT_EQ(DwRect(40, 40, 30, 30), r2);

	DwRect r3(1, 2, 3, 4);
	int x1, y1, x2, y2;
	r3.getCoords(&x1, &y1, &x2, &y2);
	EXPECT_EQ(1, x1);
	EXPECT_EQ(2, y1);
	EXPECT_EQ(3, x2);
	EXPECT_EQ(5, y2);

	r3.getRect(&x1, &y1, &x2, &y2);
	EXPECT_EQ(1, x1);
	EXPECT_EQ(2, y1);
	EXPECT_EQ(3, x2);
	EXPECT_EQ(4, y2);

	r3.setCoords(4, 5, 6, 7);
	EXPECT_EQ(DwRect(4, 5, 3, 3), r3);
	r3.setRect(5, 6, 4, 4);
	EXPECT_EQ(DwRect(5, 6, 4, 4), r3);
	r3.setSize(DwSize(10, 20));
	EXPECT_EQ(DwRect(5, 6, 10, 20), r3);

	r3.translate(4, 5);
	EXPECT_EQ(DwRect(9, 11, 10, 20), r3);
	r3.translate(DwPoint(1, 2));
	EXPECT_EQ(DwRect(10, 13, 10, 20), r3);
	EXPECT_EQ(DwRect(9, 12, 10, 20), r3.translated(-1, -1));
	EXPECT_EQ(DwRect(11, 14, 10, 20), r3.translated(DwPoint(1, 1)));
}

TEST(DwRect, operators)
{
	DwRect r0(2, 2, 10, 10);
	DwRect r1(4, 4, 20, 20);

	EXPECT_FALSE(r0 == r1);
	EXPECT_TRUE(r0 != r1);
	EXPECT_EQ(DwRect(2, 2, 22, 22), r0 | r1);
	EXPECT_EQ(DwRect(4, 4, 8, 8), r0 & r1);

	r0 |= r1;
	EXPECT_EQ(DwRect(2, 2, 22, 22), r0);

	DwRect r3(3, 3, 40, 40);
	r1 &= r3;
	EXPECT_EQ(DwRect(4, 4, 20, 20), r1);

	DwRect r4(4, 4, 4, 4);
	DwRect r5(3, 3, -3, -3);
	DwRect r6(5, 5, -3, -3);
	DwRect r7(5, 3, -3, -3);
	EXPECT_EQ(DwRect(), r4 & DwRect());
	EXPECT_EQ(DwRect(), r4 & r5);
	EXPECT_EQ(DwRect(), r5 & r4);
	EXPECT_EQ(DwRect(4, 4, 2, 2), r4 & r6);
	EXPECT_EQ(DwRect(4, 4, 2, 2), r6 & r4);
	EXPECT_EQ(DwRect(), r4 & r7);
	EXPECT_EQ(DwRect(), r7 & r4);

	EXPECT_EQ(r4, r4 | DwRect());
	EXPECT_EQ(r4, DwRect() | r4);
	EXPECT_EQ(DwRect(-1, -1, 9, 9), r4 | r5);
	EXPECT_EQ(DwRect(-1, -1, 9, 9), r5 | r4);
}

TEST(DwRect, others)
{
	DwRect r0(2, 2, 10, 10);
	DwRect r1(4, 4, 20, 20);
	DwRect r2(3, 3, 30, 30);
	DwRect r3(5, 5, 28, 28);	

	EXPECT_TRUE(r0.contains(2, 2));
	EXPECT_TRUE(r0.contains(3, 3));
	EXPECT_TRUE(r0.contains(11, 11));
	EXPECT_TRUE(r0.contains(DwPoint(11, 11)));
	EXPECT_FALSE(r0.contains(11, 11, true));
	EXPECT_FALSE(r0.contains(12, 12));
	EXPECT_FALSE(r0.contains(2, 12));
	
	EXPECT_FALSE(r0.contains(r1));
	EXPECT_TRUE(r2.contains(r1));
	EXPECT_TRUE(r2.contains(r3));
	EXPECT_FALSE(r2.contains(r3, true));

	EXPECT_FALSE(r3.contains(DwRect()));
	EXPECT_FALSE(DwRect().contains(r3));
	EXPECT_FALSE(r3.contains(33, 33, true));
	EXPECT_FALSE(r3.contains(23, 33, true));

	EXPECT_EQ(DwRect(2, 2, 22, 22), r0.united(r1));
	EXPECT_EQ(DwRect(4, 4, 8, 8), r0.intersected(r1));

	EXPECT_TRUE(r0.intersects(r1));

	DwRect r4(6, 6, 10, 10);
	r4.adjust(1, 1, -1, -1);
	EXPECT_EQ(DwRect(7, 7, 8, 8), r4);	
	EXPECT_EQ(DwRect(6, 6, 10, 10), r4.adjusted(-1, -1, 1, 1));

	DwRect r5(1, 2, 10, 20);
	EXPECT_EQ(DwPoint(5, 11), r5.center());

	DwRect r6(5, 5, -10, -10);
	EXPECT_TRUE(r6.contains(4, 4));

	DwRect r7 = r6.normalized();
	EXPECT_EQ(DwRect(-6, -6, 12, 12), r7);
	EXPECT_EQ(DwRect(-6, -6, 12, 12), r7.normalized());
}


/************************************************************************
	DwRectF
************************************************************************/


TEST(DwRectF, DwRectF)
{
	DwRectF r0;
	EXPECT_TRUE(r0.isNull());
	EXPECT_TRUE(r0.isEmpty());
	EXPECT_FALSE(r0.isValid());

	DwRectF r1(1.1, 1.1, 10.1, 10.1);
	EXPECT_FALSE(r1.isNull());
	EXPECT_FALSE(r1.isEmpty());
	EXPECT_TRUE(r1.isValid());

	DwRectF r2(5.5, 5.5, -10.5, -10.5);
	EXPECT_FALSE(r2.isNull());
	EXPECT_TRUE(r2.isEmpty());
	EXPECT_FALSE(r2.isValid());

	DwRectF r3(-1.2, -1.2, 10.2, 10.2);
	EXPECT_FALSE(r3.isNull());
	EXPECT_FALSE(r3.isEmpty());
	EXPECT_TRUE(r3.isValid());

	DwRectF r4(1.1, -1.1, 10.1, -10.1);
	EXPECT_FALSE(r4.isNull());
	EXPECT_TRUE(r4.isEmpty());
	EXPECT_FALSE(r4.isValid());

	DwRectF r5(DwPointF(3.3, 3.3), DwSizeF(10.5, 10.5));
	DwRectF r6(DwPointF(3.3, 3.3), DwPointF(13.8, 13.8));
	EXPECT_EQ(r5, r6);
}

TEST(DwRectF, values)
{
	DwRectF r0(1.1, 2.2, 10.1, 11.2);
	EXPECT_DOUBLE_EQ(1.1, r0.x());
	EXPECT_DOUBLE_EQ(2.2, r0.y());
	EXPECT_DOUBLE_EQ(10.1, r0.width());
	EXPECT_DOUBLE_EQ(11.2, r0.height());
	EXPECT_DOUBLE_EQ(1.1, r0.left());
	EXPECT_DOUBLE_EQ(11.2, r0.right());
	EXPECT_DOUBLE_EQ(2.2, r0.top());
	EXPECT_DOUBLE_EQ(13.4, r0.bottom());
	EXPECT_EQ(DwPointF(1.1, 13.4), r0.bottomLeft());
	EXPECT_EQ(DwPointF(11.2, 13.4), r0.bottomRight());
	EXPECT_EQ(DwPointF(1.1, 2.2), r0.topLeft());
	EXPECT_EQ(DwPointF(11.2, 2.2), r0.topRight());

	DwRectF r1(DwPointF(2.2, 3.3), DwSizeF(12.2, 13.3));

	r1.setX(40.4);
	r1.setY(60.6);
	r1.setWidth(400.4);
	r1.setHeight(600.6);
	EXPECT_DOUBLE_EQ(40.4, r1.x());
	EXPECT_DOUBLE_EQ(60.6, r1.y());
	EXPECT_DOUBLE_EQ(400.4, r1.width());
	EXPECT_DOUBLE_EQ(600.6, r1.height());
	EXPECT_DOUBLE_EQ(40.4, r1.left());
	EXPECT_DOUBLE_EQ(60.6, r1.top());
	EXPECT_DOUBLE_EQ(440.8, r1.right());
	EXPECT_DOUBLE_EQ(661.2, r1.bottom());

	r1.moveLeft(30.3);
	EXPECT_EQ(DwPointF(30.3, 60.6), r1.topLeft());
	EXPECT_EQ(DwPointF(430.7, 661.2), r1.bottomRight());

	r1.moveRight(430.8);
	EXPECT_EQ(DwPointF(30.4, 60.6), r1.topLeft());
	EXPECT_EQ(DwPointF(430.8, 661.2), r1.bottomRight());

	r1.moveCenter(DwPointF(340, 460));
	EXPECT_EQ(DwPointF(139.8, 159.7), r1.topLeft());
	EXPECT_EQ(DwPointF(540.2, 760.3), r1.bottomRight());

	r1.moveTo(30, 50);
	EXPECT_EQ(DwPointF(30, 50), r1.topLeft());
	EXPECT_EQ(DwPointF(430.4, 650.6), r1.bottomRight());

	r1.moveTo(DwPointF(20, 40));
	EXPECT_EQ(DwPointF(20, 40), r1.topLeft());
	EXPECT_EQ(DwPointF(420.4, 640.6), r1.bottomRight());

	r1.moveTop(10);
	EXPECT_EQ(DwPointF(20, 10), r1.topLeft());
	EXPECT_EQ(DwPointF(420.4, 610.6), r1.bottomRight());

	r1.moveTopLeft(DwPointF(10.1, 10.1));
	EXPECT_EQ(DwPointF(10.1, 10.1), r1.topLeft());
	EXPECT_EQ(DwPointF(410.5, 610.7), r1.bottomRight());

	r1.moveTopRight(DwPointF(419.5, 20.7));
	EXPECT_EQ(DwPointF(19.1, 20.7), r1.topLeft());
	EXPECT_EQ(DwPointF(419.5, 621.3), r1.bottomRight());

	r1.moveBottom(619.6);
	EXPECT_EQ(DwPointF(19.1, 19), r1.topLeft());
	EXPECT_EQ(DwPointF(419.5, 619.6), r1.bottomRight());

	r1.moveBottomLeft(DwPointF(40.1, 639.7));
	EXPECT_EQ(DwPointF(40.1, 39.1), r1.topLeft());
	EXPECT_EQ(DwPointF(440.5, 639.7), r1.bottomRight());

	r1.moveBottomRight(DwPointF(449.4, 649.4));
	EXPECT_EQ(DwPointF(49, 48.8), r1.topLeft());
	EXPECT_EQ(DwPointF(449.4, 649.4), r1.bottomRight());

	EXPECT_EQ(DwSizeF(400.4, 600.6), r1.size());

	DwRectF r2(10.5, 10.5, 100, 100);
	r2.setLeft(20.5);
	EXPECT_EQ(DwRectF(20.5, 10.5, 90, 100), r2);
	r2.setTop(20.5);
	EXPECT_EQ(DwRectF(20.5, 20.5, 90, 90), r2);
	r2.setRight(100.5);
	EXPECT_EQ(DwRectF(20.5, 20.5, 80, 90), r2);
	r2.setBottom(100.5);
	EXPECT_EQ(DwRectF(20.5, 20.5, 80, 80), r2);
	r2.setTopLeft(DwPointF(30.5, 30.5));
	EXPECT_EQ(DwRectF(30.5, 30.5, 70, 70), r2);
	r2.setTopRight(DwPointF(90.5, 40.5));
	EXPECT_EQ(DwRectF(30.5, 40.5, 60, 60), r2);
	r2.setBottomLeft(DwPointF(40.5, 80.5));
	EXPECT_EQ(DwRectF(40.5, 40.5, 50, 40), r2);
	r2.setBottomRight(DwPointF(70.5, 70.5));
	EXPECT_EQ(DwRectF(40.5, 40.5, 30, 30), r2);

	DwRectF r3(1.1, 2.2, 3.3, 4.4);
	double x1, y1, x2, y2;
	r3.getCoords(&x1, &y1, &x2, &y2);
	EXPECT_DOUBLE_EQ(1.1, x1);
	EXPECT_DOUBLE_EQ(2.2, y1);
	EXPECT_DOUBLE_EQ(4.4, x2);
	EXPECT_DOUBLE_EQ(6.6, y2);

	r3.getRect(&x1, &y1, &x2, &y2);
	EXPECT_DOUBLE_EQ(1.1, x1);
	EXPECT_DOUBLE_EQ(2.2, y1);
	EXPECT_DOUBLE_EQ(3.3, x2);
	EXPECT_DOUBLE_EQ(4.4, y2);
	
	r3.setCoords(0.1, 0.2, 0.3, 0.4);
	EXPECT_EQ(DwRectF(0.1, 0.2, 0.2, 0.2), r3);
	r3.setRect(0.1, 0.2, 0.3, 0.4);
	EXPECT_EQ(DwRectF(0.1, 0.2, 0.3, 0.4), r3);
	r3.setSize(DwSizeF(10, 20));
	EXPECT_EQ(DwRectF(0.1, 0.2, 10, 20), r3);

	r3.translate(4.1, 5.2);
	EXPECT_EQ(DwRectF(4.2, 5.4, 10, 20), r3);
	r3.translate(DwPointF(2.1, 2.2));
	EXPECT_EQ(DwRectF(6.3, 7.6, 10, 20), r3);
	EXPECT_EQ(DwRectF(7.4, 8.7, 10, 20), r3.translated(1.1, 1.1));
	EXPECT_EQ(DwRectF(7.4, 8.7, 10, 20), r3.translated(DwPointF(1.1, 1.1)));
}

TEST(DwRectF, operators)
{
	DwRectF r0(2.2, 2.2, 10.1, 10.1);
	DwRectF r1(4.4, 4.4, 20.2, 20.2);

	EXPECT_FALSE(r0 == r1);
	EXPECT_TRUE(r0 != r1);
	EXPECT_EQ(DwRectF(2.2, 2.2, 22.4, 22.4), r0 | r1);
	EXPECT_EQ(DwRectF(4.4, 4.4, 7.9, 7.9), r0 & r1);

	r0 |= r1;
	EXPECT_EQ(DwRectF(2.2, 2.2, 22.4, 22.4), r0);

	DwRectF r3(3.3, 3.3, 40.4, 40.4);
	r1 &= r3;
	EXPECT_EQ(DwRectF(4.4, 4.4, 20.2, 20.2), r1);

	DwRectF r4(4, 4, 4, 4);
	DwRectF r5(3, 3, -3, -3);
	DwRectF r6(5, 5, -3, -3);
	DwRectF r7(5, 3, -3, -3);
	EXPECT_EQ(DwRectF(), r4 & DwRectF());
	EXPECT_EQ(DwRectF(), r4 & r5);
	EXPECT_EQ(DwRectF(), r5 & r4);
	EXPECT_EQ(DwRectF(4, 4, 1, 1), r4 & r6);
	EXPECT_EQ(DwRectF(4, 4, 1, 1), r6 & r4);
	EXPECT_EQ(DwRectF(), r4 & r7);
	EXPECT_EQ(DwRectF(), r7 & r4);

	EXPECT_EQ(r4, r4 | DwRectF());
	EXPECT_EQ(r4, DwRectF() | r4);
	EXPECT_EQ(DwRectF(0, 0, 8, 8), r4 | r5);
	EXPECT_EQ(DwRectF(0, 0, 8, 8), r5 | r4);
}

TEST(DwRectF, others)
{
	DwRectF r0(2.2, 2.2, 10.1, 10.1);
	DwRectF r1(4.4, 4.4, 20.2, 20.2);
	DwRectF r2(3.3, 3.3, 30.3, 30.3);
	DwRectF r3(5.5, 5.5, 27.7, 27.7);

	EXPECT_TRUE(r0.contains(2.2, 2.2));
	EXPECT_TRUE(r0.contains(3.3, 3.3));
	EXPECT_TRUE(r0.contains(11.1, 11.1));
	EXPECT_TRUE(r0.contains(DwPointF(12.1, 12.1)));
	EXPECT_TRUE(r0.contains(12.2, 12.3));
	EXPECT_FALSE(r0.contains(2.3, 12.4));

	EXPECT_FALSE(r0.contains(r1));
	EXPECT_TRUE(r2.contains(r1));
	EXPECT_TRUE(r2.contains(r3));

	EXPECT_FALSE(r3.contains(DwRectF()));
	EXPECT_FALSE(DwRectF().contains(r3));

	EXPECT_EQ(DwRectF(2.2, 2.2, 22.4, 22.4), r0.united(r1));
	EXPECT_EQ(DwRectF(4.4, 4.4, 7.9, 7.9), r0.intersected(r1));

	EXPECT_TRUE(r0.intersects(r1));

	DwRectF r4(4.4, 5.6, 7.1, 8.2);
	EXPECT_EQ(DwRect(4, 6, 7, 8), r4.toRect());
	EXPECT_EQ(DwRect(4, 5, 8, 9), r4.toAlignedRect());

	DwRectF r5(6.1, 6.1, 10.1, 10.1);
	r5.adjust(0.1, 0.1, -0.1, -0.1);
	EXPECT_EQ(DwRectF(6.2, 6.2, 9.9, 9.9), r5);	
	EXPECT_EQ(DwRectF(6.1, 6.1, 10.1, 10.1), r5.adjusted(-0.1, -0.1, 0.1, 0.1));

	DwRectF r6(1.1, 2.2, 10, 20);
	EXPECT_EQ(DwPointF(6.1, 12.2), r6.center());

	DwRectF r7(5.5, 5.5, -10.5, -10.5);
	EXPECT_TRUE(r7.contains(4.4, 4.4));

	DwRectF r8 = r7.normalized();
	EXPECT_EQ(DwRectF(-5, -5, 10.5, 10.5), r8);
	EXPECT_EQ(DwRectF(-5, -5, 10.5, 10.5), r8.normalized());
}
