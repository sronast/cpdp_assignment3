# Online tic-tac-toe Server 

### Contributors Ronast Subedi(rs22ce) and Saransh Bogati(sb23bu)

### Please read the instructions carefully before running the program

## Required dependenices

<ol>
    <li>Linux OS</li>
    <li>**make** tool</li>
    <li>STD C++20</li>
    <li>**telnet** client</li>
</ol>

## Instructions for compiling the program

In the terminal, enter the make command:
```make```

## Instructions for running the program

1. Start the server, accepted ports are in the range [50000, 60000]
```./server PORT```

2. Connect to the server form multiple client devices
```telnet SERVER_IP SERVER_PORT```

3. Play the game

## Supported functionalities
[...] optional field, <......> required field
<ol>
    
<li>who # List all online users</li>
<li>stats [name] # Display user information</li>
<li>game # list all current games</li>
<li>observe <game_num> # Observe a game</li>
<li>unobserve # Unobserve a game</li>
<li>match <name> <b|w> [t] # Try to start a game</li>
<li><A|B|C><1|2|3> # Make a move in a game</li>
<li>resign # Resign a game</li>
<li>refresh # Refresh a game</li>
<li>shout <msg> # shout <msg> to every one online</li>
<li>tell <name> <msg> # tell user <name> message</li>
<li>kibitz <msg> # Comment on a game when observing</li>
<li>’ <msg> # Comment on a game</li>
<li>quiet # Quiet mode, no broadcast messages</li>
<li>nonquiet # Non-quiet mode</li>
<li>block <id> # No more communication from <id></li>
<li>unblock <id> # Allow communication from <id></li>
<li>listmail # List the header of the mails</li>
<li>readmail <msg_num> # Read the particular mail</li>
<li>deletemail <msg_num> # Delete the particular mail</li>
<li>mail <id> <title> # Send id a mail</li>
<li>info <msg> # change your information to <msg></li>
<li>passwd <new> # change password</li>
<li>exit # quit the system</li>
<li>quit # quit the system</li>
<li>help # print this message</li>
<li>? # print this message</li>
<li>register <name> <pwd> # register a new user</li>
</ol>
