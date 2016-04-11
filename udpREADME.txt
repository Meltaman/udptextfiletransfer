To use the udp client/server ncp rcv:

1. Put both files into a directory.

2. Have a file with contents you wish to copy also in the directory.

3. Compile udp and ncp with gcc -o ncp ncp.c and gcc -o rcv rcv.c

4. Open a second terminal window and navigate to the directory the executables are now located in.

5. Execute rcv first by typing ./rcv and hitting return. Terminal should display "Receiving ..."

6. Next go to the other terminal window.

7. Type ./ncp yoursourcefile yourdestinationfile@computername and hit return.

8. ncp should state that it is sending the source file to the destination file, 
   the size of the source file, and the time to send.

9. ncp will stop executing once it sends a single file. To send a new file, run the program again

10. rcv should display packets being transferred, their size, and time to receive them

11. rcv will remain waiting for new transfers. ctrl + c to exit rcv program. 
    If you wish to send more data to the SAME FILE you can continue to let this loop and send from the ncp function.

    If you wish to send more data to a NEW FILE you will need to ctrl + c out of rcv and re execute it before you send
    new files from ncp.