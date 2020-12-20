OBJS     = showmsg.o ../../lcd.o
CPPFLAGS = -I../../
LDFLAGS  = -lpigpio

showmsg: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: install
install: showmsg
	ln -s $(PWD)/showmsg /usr/local/bin/lcd-showmsg
	ln -s $(PWD)/showmsg.service /etc/systemd/system/lcd-showmsg.service
	systemctl enable lcd-showmsg.service
