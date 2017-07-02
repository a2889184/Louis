from polyglot.downloader import downloader
download_list = downloader.supported_tasks(lang="en")

for i in range(0, len(download_list)):
	name = str(download_list[i])
	downloader.download(name)
