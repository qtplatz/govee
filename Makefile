
sync:
	rsync -arzuv mars:/var/log/govee-data/ ./data/

.PHONY: sync
