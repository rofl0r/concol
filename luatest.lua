local con = require("libconcol")

con.init()
con.init_graphics(800, 600)

con.setcolor(1, con.rgb(255,255,255))

con.goto(10, 20)
con.putchar("x", 1)

con.getkey()

con.cleanup()
