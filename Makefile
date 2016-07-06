all:publisher consumer

publisher:
	cd Publisher; $(MAKE)
	
consumer:
	cd Consumer; $(MAKE)

clean:
	rm Publisher/publisher Consumer/consumer
