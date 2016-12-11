NAME = ccc-nano-drone

PresentationSubdir = presentation

MediasToLink = video.mp4


presentation: $(NAME).rst $(NAME).css
	hovercraft -s $(NAME).rst $(PresentationSubdir)
	$(foreach media, $(MediasToLink), ln -s ../$(media) $(PresentationSubdir)/$(media) )

show: presentation
	$(BROWSER) $(PresentationSubdir)/index.html

clean:
	rm -rf $(PresentationSubdir)

