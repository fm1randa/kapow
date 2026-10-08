# Kapow

A punch clock that tracks hours worked per project and which of those hours are already billed.

## Language

**Project**:
A named bucket of tracked work; projects can nest inside other projects.
_Avoid_: Client, job

**Timer**:
The running clock of a single project, from its start time until it is stopped or cancelled. Each project has at most one timer, but several projects can have timers running at once.
_Avoid_: Stopwatch, cronômetro (in code and docs)

**Start time**:
The date and time a timer counts from. It can be changed while the timer runs.
_Avoid_: Clock-in time

**Session**:
A finished, recorded span of work in a project, with a date, a start, a stop and a task. A timer becomes a session when it is stopped.
_Avoid_: Entry, punch, record

**Task**:
The free-text note describing what a session or timer is about.
_Avoid_: Note, description

**Billed**:
A mark on a session meaning it and every earlier session were already charged to the client; billed sessions are frozen.
_Avoid_: Invoiced, paid
