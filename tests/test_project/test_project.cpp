/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "test_project.h"

#include "project.h"

#include <QTest>
#include <QTreeWidget>

//-----------------------------------------------------------------------------

void TestProject::startTimeOfRunningTimer()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");
	QVERIFY(!project->startTime().isValid());

	const QDateTime start(QDate(2026, 10, 8), QTime(9, 10, 0));
	QVERIFY(project->start(start));
	QCOMPARE(project->startTime(), start);
}

//-----------------------------------------------------------------------------

void TestProject::startTimeAfterStop()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 8), QTime(9, 10, 0))));
	QVERIFY(project->stop(QDateTime(QDate(2026, 10, 8), QTime(9, 40, 0))));
	QVERIFY(!project->startTime().isValid());
}

//-----------------------------------------------------------------------------

void TestProject::startTimeAfterCancel()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 8), QTime(9, 10, 0))));
	QVERIFY(project->stop());
	QVERIFY(!project->startTime().isValid());
}

//-----------------------------------------------------------------------------

QTEST_MAIN(TestProject)

#include "moc_test_project.cpp"
