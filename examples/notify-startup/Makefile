OBJS     = showmsg.o ../../lcd.o
CPPFLAGS = -I../../
LDFLAGS  = -lpigpio

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir  := $(patsubst %/,%,$(dir $(mkfile_path)))

showmsg: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: install
install: showmsg
	cp $(mkfile_dir)/showmsg /usr/local/bin/lcd-showmsg
	cp $(mkfile_dir)/showmsg.service /etc/systemd/system/lcd-showmsg.service
	systemctl daemon-reload
	systemctl enable lcd-showmsg.service
