# Decorator, Chain of Responsibility, Observer - Coffee Shack
### Description
Part one: Decorator design pattern. Decorating a component class named Drink by wrapping it inside of ingredient decorator classes. These append ingredients to the drink name.

Part two: Chain of Responsibility design pattern. An abstract Barista class has derived Baristas named Junior, Senior, and Manager. Each Barista has limitations on the type
of ingredients they can add. If a certain Barista can't take an order, it passes the request to the next role in the chain.

Part three: Observer design pattern. Customers can now take orders one at a time and Baristas notify all Customers when an order is ready. There is a 33% chance of a Barista
finishing their order during each iteration. Customers are observers which register with a Barista to receive notifications. When they recieve notifications, Customers
determine if the order is theirs and will pick it up if it is their name.

Usage: ./CoffeeShack.out

### Example output:

![Output](https://i.imgur.com/UTe6RMA.png)
