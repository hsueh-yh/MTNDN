all:consumer publisher
	
consumer:
	cd Consumer; $(MAKE)

publisher:
	cd Publisher; $(MAKE)

clean:
	rm Consumer/consumer Publisher/publisher
