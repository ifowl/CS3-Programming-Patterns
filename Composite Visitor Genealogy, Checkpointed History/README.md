# Composite Visitor Genealogy, Checkpointed History
### Description
Part one: Composite and Visitor design patterns. Concrete RelationalCheck visitor class created to traverse and genealogy tree of Men and Women. Two full names are taken
from the command line and if they are deteremined to be siblings, parents, children, aunts, or uncles, then they are deemed to be "related". (For whatever reason
Grandparents, nephews, and nieces do not count.)

Part two: Command and Memento design patterns. Client code was modified to include a history of commands made, redoing commands, and creating checkpoints. Checkpoints
purge history, as does rolling back to a checkpoint.

Usage: ./genealogyTree.out or ./history.out

### Example output:

![Output](https://i.imgur.com/foInmCW.png)
![Output](https://i.imgur.com/uHSEvn3.png)
![Output](https://i.imgur.com/ML96YY5.png)
![Output](https://i.imgur.com/cKvyJUI.png)
