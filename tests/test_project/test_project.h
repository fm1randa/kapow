/*
	SPDX-FileCopyrightText: 2026 Filipe Miranda

	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef KAPOW_TEST_PROJECT_H
#define KAPOW_TEST_PROJECT_H

#include <QObject>

class TestProject : public QObject
{
	Q_OBJECT

private Q_SLOTS:
	void startTimeOfRunningTimer();
	void startTimeAfterStop();
	void startTimeAfterCancel();
};

#endif // KAPOW_TEST_PROJECT_H
