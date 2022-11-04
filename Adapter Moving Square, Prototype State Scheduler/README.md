# Adapter Moving Square, Prototype State Scheduler
### Description
Part one: Adapter design pattern. Uses a Square adapter with multiple inheritance from the adaptee LegacyRectangle and base interface of Figure. The adapter users adaptee
functions to implement the desired behavior of resizing a square.

Part two: State and Prototype design patterns. Implements a three state model and process queues to simulate running processes. Process can have a state of ready, running,
or blocked and must wait in ready queue and blocked queue. New processes may be created by cloning the state from a prototype.

Usage: ./AdapterClass.out or ./PrototypeScheduler.out

### Example output:

![Output](https://i.imgur.com/2ARpAjM.png)
![Output](https://i.imgur.com/q6i7h0X.png)
![Output](https://i.imgur.com/L5Dnxwi.png)
