# Automated Parking Lot
I made a program that simulates FATA Automated Parking Systems 
(you can find more info about it in this video: https://www.youtube.com/watch?v=VwS1QwXqgpk&ab_channel=FATAAutomation).
This program is able to register new customers, the quantity of vehicles that each customers has and the name and model of every one of them.
You can park your cars on different levels based on the time the car is going to stay parked, as follows: <= 1 day gets parked on the first floor,
<= 3 days gets parked on the second floor, <= 5 days gets parked on the third floor, > 5 days on the fourth floor, and so on. Also, you can simulate
unparking a certain car just by providing your ID and the name of the car to be unparked. You can also view details about the signed in customers and
the cars they own.
To achieve this, I use different data structures such as lists, queues and stacks. Lists and queues are managed dinamically, stacks have a static size 
of 3 available spaces. I use this data structures in a nested way, I mean, I have a main list which its data field is a pointer to a queue and the data
field of the queue is a stack of three elements.
