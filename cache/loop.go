package "cache"

import ()

func (c *Cache) loop() {
	for {
		select {
		case <-c.stop:
			return