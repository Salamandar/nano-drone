:title:         How to train (and reprogram, actually) your quadcopter
:author:        Salamandar
:css:           ccc-nano-drone.css

----

How to train (and reprogram, actually) your Quadcopter !
========================================================
:id: first-slide

----


About me
========
.. image:: ./images/Salamander.png
    :align: left
    :height: 210px

* (very) young Physics researcher \& C++ dev

* fee

* fii

* foo


----

Goals
=====

* Reverse engineering

  + the electronics

  + the radio protocol

* Improve the quadcopter ?

    + Asservissement

    + Fun tweaks

    + Bluetooth control ?

* Learn about electronics

* Start a code base for other potential projects (drones, IOT)

* Have fun !


----

Rev-engineering the electronics
===================================

.. raw:: html

    <embed>
        <video controls preload="auto" height=600 width=auto
            src="./video.mp4" />
    </embed>




----

Flashing the software
==========================


----

Debugging
===========


----

Is it legal ?
==============

----

Conclusion
============













----

First header
============

Second header
-------------

Third header
............

- There is *Emphasis* and **strong emphasis**, rendered as <em> and <strong>.

#. Numbered lists are of course also supported.

#. ``inline literals``, rendered as <tt> and usually shown with a monospace font, which is good for source code.

#. Hyperlinks, like Python_

.. _Python: http://www.python.org


.. note::

    And then you indent the text afterwards. You can have a lot of formatting
    in the presenter notes, like *emphasis* and **strong** emphasis.

    - Even bullet lists!

    - Which can be handy!

    But you can't have any headings.


If you want a whole block of preformatted text you can use double colons::

    And then you need to indent the block of text that should be preformatted

You can even have the double colons on a line by themselves:

::

    And this text will now be rendered as preformatted text

.. code:: python

    def day_of_year(month, day):
        return (month - 1) * 30 + day_of_month

