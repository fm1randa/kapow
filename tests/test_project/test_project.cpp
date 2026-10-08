/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "test_project.h"

#include "project.h"
#include "session.h"
#include "session_model.h"

#include <QTest>
#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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

void TestProject::setStartTimeEarlierThenStop()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 8), QTime(9, 25, 0))));
	const QDateTime start(QDate(2026, 10, 8), QTime(9, 10, 0));
	const QDateTime now(QDate(2026, 10, 8), QTime(9, 30, 0));
	QVERIFY(project->setStartTime(start, now));
	QCOMPARE(project->startTime(), start);
	QCOMPARE(project->time(), QString("00:20:00"));

	QVERIFY(project->stop(QDateTime(QDate(2026, 10, 8), QTime(9, 40, 0))));
	QCOMPARE(project->model()->rowCount() - 1, 1);
	QCOMPARE(project->model()->session(0), Session(QDate(2026, 10, 8), QTime(9, 10, 0), QTime(9, 40, 0), QString(), false));
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeWithoutRunningTimer()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	const QDateTime start(QDate(2026, 10, 8), QTime(9, 10, 0));
	const QDateTime now(QDate(2026, 10, 8), QTime(9, 30, 0));
	QVERIFY(!project->setStartTime(start, now));
	QVERIFY(!project->startTime().isValid());
	QCOMPARE(project->time(), QString());
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeInFuture()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	const QDateTime old_start(QDate(2026, 10, 8), QTime(9, 10, 0));
	QVERIFY(project->start(old_start));
	const QDateTime now(QDate(2026, 10, 8), QTime(9, 30, 0));
	project->updateTime(now);
	QVERIFY(!project->setStartTime(QDateTime(QDate(2026, 10, 8), QTime(9, 30, 1)), now));
	QCOMPARE(project->startTime(), old_start);
	QCOMPARE(project->time(), QString("00:20:00"));
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeBeforeLastSession()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");
	QVERIFY(project->model()->add(QDateTime(QDate(2026, 10, 8), QTime(8, 0, 0)), QDateTime(QDate(2026, 10, 8), QTime(8, 30, 0)), QString()));
	QVERIFY(project->model()->add(QDateTime(QDate(2026, 10, 8), QTime(9, 0, 0)), QDateTime(QDate(2026, 10, 8), QTime(9, 5, 0)), QString()));

	const QDateTime old_start(QDate(2026, 10, 8), QTime(9, 25, 0));
	QVERIFY(project->start(old_start));
	const QDateTime now(QDate(2026, 10, 8), QTime(9, 30, 0));

	// In the gap between the two sessions
	QVERIFY(!project->setStartTime(QDateTime(QDate(2026, 10, 8), QTime(8, 45, 0)), now));
	QCOMPARE(project->startTime(), old_start);

	// One second before the last session ends
	QVERIFY(!project->setStartTime(QDateTime(QDate(2026, 10, 8), QTime(9, 4, 59)), now));
	QCOMPARE(project->startTime(), old_start);
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeAtEndOfLastSession()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");
	QVERIFY(project->model()->add(QDateTime(QDate(2026, 10, 8), QTime(8, 0, 0)), QDateTime(QDate(2026, 10, 8), QTime(9, 0, 0)), QString()));

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 8), QTime(9, 25, 0))));
	const QDateTime start(QDate(2026, 10, 8), QTime(9, 0, 0));
	QVERIFY(project->setStartTime(start, QDateTime(QDate(2026, 10, 8), QTime(9, 30, 0))));
	QCOMPARE(project->startTime(), start);

	QVERIFY(project->stop(QDateTime(QDate(2026, 10, 8), QTime(9, 40, 0))));
	QCOMPARE(project->model()->rowCount() - 1, 2);
	QCOMPARE(project->model()->session(1), Session(QDate(2026, 10, 8), QTime(9, 0, 0), QTime(9, 40, 0), QString(), false));
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeLater()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 8), QTime(9, 0, 0))));
	const QDateTime now(QDate(2026, 10, 8), QTime(9, 30, 0));

	const QDateTime later(QDate(2026, 10, 8), QTime(9, 20, 0));
	QVERIFY(project->setStartTime(later, now));
	QCOMPARE(project->startTime(), later);
	QCOMPARE(project->time(), QString("00:10:00"));

	// Up to now is allowed
	QVERIFY(project->setStartTime(now, now));
	QCOMPARE(project->startTime(), now);
	QCOMPARE(project->time(), QString("00:00:00"));
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeAcrossMidnight()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 9), QTime(0, 30, 0))));
	const QDateTime start(QDate(2026, 10, 8), QTime(23, 0, 0));
	QVERIFY(project->setStartTime(start, QDateTime(QDate(2026, 10, 9), QTime(0, 45, 0))));
	QCOMPARE(project->startTime(), start);
	QCOMPARE(project->time(), QString("01:45:00"));

	QVERIFY(project->stop(QDateTime(QDate(2026, 10, 9), QTime(1, 0, 0))));
	QCOMPARE(project->model()->rowCount() - 1, 2);
	QCOMPARE(project->model()->session(0), Session(QDate(2026, 10, 8), QTime(23, 0, 0), QTime(23, 59, 59), QString(), false));
	QCOMPARE(project->model()->session(1), Session(QDate(2026, 10, 9), QTime(0, 0, 0), QTime(1, 0, 0), QString(), false));
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeWithoutSessions()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 8), QTime(9, 0, 0))));
	const QDateTime start(QDate(2020, 1, 1), QTime(0, 0, 0));
	QVERIFY(project->setStartTime(start, QDateTime(QDate(2026, 10, 8), QTime(9, 30, 0))));
	QCOMPARE(project->startTime(), start);
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeKeepsMaximumDateTime()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 8), QTime(9, 25, 0))));
	QVERIFY(project->setStartTime(QDateTime(QDate(2026, 10, 8), QTime(9, 0, 0)), QDateTime(QDate(2026, 10, 8), QTime(9, 30, 0))));

	// Sessions may not reach into the running timer
	QVERIFY(!project->model()->add(QDateTime(QDate(2026, 10, 8), QTime(9, 5, 0)), QDateTime(QDate(2026, 10, 8), QTime(9, 10, 0)), QString()));
	QVERIFY(project->model()->add(QDateTime(QDate(2026, 10, 8), QTime(8, 0, 0)), QDateTime(QDate(2026, 10, 8), QTime(8, 30, 0)), QString()));
}

//-----------------------------------------------------------------------------

void TestProject::setStartTimeIsAutosaved()
{
	QTreeWidget tree;
	Project* project = new Project(&tree, "Project");

	QVERIFY(project->start(QDateTime(QDate(2026, 10, 8), QTime(9, 25, 0))));
	QVERIFY(project->setStartTime(QDateTime(QDate(2026, 10, 8), QTime(9, 10, 0)), QDateTime(QDate(2026, 10, 8), QTime(9, 30, 0))));

	QString data;
	QXmlStreamWriter writer(&data);
	project->toXml(writer);

	QXmlStreamReader reader(data);
	QString start;
	while (!reader.atEnd()) {
		if (reader.readNextStartElement() && (reader.name() == QLatin1String("autosave"))) {
			start = reader.attributes().value(QLatin1String("start")).toString();
			break;
		}
	}
	QCOMPARE(start, QString("2026-10-08T09:10:00"));
}

//-----------------------------------------------------------------------------

QTEST_MAIN(TestProject)

#include "moc_test_project.cpp"
