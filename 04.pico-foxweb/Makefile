all: PICOFoxweb

clean:
	@rm -rf *.o
	@rm -rf PICOFoxweb

install: PICOFoxweb
	useradd -c "PICOFoxweb user" -r -s /sbin/nologin -d /var/www/picoweb picofoxweb
	install -o root -g root -m 0755 PICOFoxweb /usr/local/sbin/
	install -o root -g root -m 0644 pico.service /etc/systemd/system/
	systemctl daemon-reload
	systemctl restart pico.service
	mkdir -p /var/www/picoweb
	cp -r webroot -t /var/www/picoweb/
	chown -R picofoxweb:picofoxweb /var/www/picoweb

uninstall:
	systemctl stop pico
	rm -rf /var/www/picoweb
	rm -f /usr/local/sbin/PICOFoxweb
	rm -f /etc/systemd/system/pico.service
	systemctl daemon-reload
	userdel -f picofoxweb

PICOFoxweb: main.o httpd.o
	gcc -o PICOFoxweb $^

main.o: main.c httpd.h
	gcc -c -o main.o main.c

httpd.o: httpd.c httpd.h
	gcc -c -o httpd.o httpd.c
