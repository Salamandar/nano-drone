NAME = ccc-nano-drone

PresentationSubdir = presentation

MediasToLink = video.mp4 imperial_march.mp4


presentation: $(NAME).rst $(NAME).css
	hovercraft -s $(NAME).rst $(PresentationSubdir)
	$(foreach media, $(MediasToLink), ln -fs ../$(media) $(PresentationSubdir)/$(media) )

show: presentation
	$(BROWSER) $(PresentationSubdir)/index.html

clean:
	rm -rf $(PresentationSubdir)

