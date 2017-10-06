# Extractor

### Known Issues

CLD2 detects almost 300 different languages when processing Common Crawl. Since each worker opens a new file for every detected language, the total number of opened files might exceed the user's hard or soft resource limits on open file descriptors. You can type ```ulimit -a``` to get a list of all current resource limits.

For the Common Crawl purposes, executing the following command should increase the limit of open file descriptors sufficiently:
```bash
ulimit -n 32768
```
