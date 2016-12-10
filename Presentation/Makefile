NAME = ccc-nano-drone

presentation: $(NAME).rst $(NAME).css
	hovercraft $(NAME).rst presentation

show: presentation
	$(BROWSER) presentation/index.html

clean:
	rm -rf presentation

