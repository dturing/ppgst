ppgst
=====

Wrapper library to write GStreamer Plugins in C++, and a number of plugins.


## What is it?

libppgst is a wrapper library to write GStreamer plugins in C++.
It tries to make it easier to write cool, mostly video-processing elements
for the multimedia framework of choice. libppgst applies some C++ template
magic to separate "Processing Model" from "Data Model", effectively allowing
for various base classes that implement different processing paradigms
(Inplace Filter, Producer, Consumer, etc). These base classes are templated
to "Formats", that is, abstractions to access GstBuffers and GstCaps of a
certain format.


## Why is it called ppgst?

It was formerly called libWarsaw for historical reasons. ppgst is a variant
of ++gst, playing on gstmm etc. If you don't get it, don't worry. It's stupid.


## How does it work?

ppgst elements derive from GStreamer's GstBase* classes,
effectively leaving a lot of scheduling details to the GStreamer team.
Before that, there was one base class (wElement), a C++ class that is
also a GObject class deriving from GstElement. Now, wElement is somewhat more
complex in that it allows (template magic again) it's derived classes to 
pretend to be GObject classes derived from various GStreamer base classes.
Weird? Yes, but it works. If you really care, look at classes like 
wGstBaseTransform. It's magic :)

If you just want to write elements, you don't have to care for this
GObject/C++ magic. Just derive from one of the Processing Model Classes,
specify (via templates) the Data Formats you want to handle, and off you go.
You could look at the test/* classes to get an idea how that works. It's
pretty simple.


## Is it stable?

Do I care? It works for me.


## Installation

./configure && make && make install


## Credits

While libppgst is a one-person project, it wouldn't have been possible
without the constant support of a few friendly and generous individuals
and institutions. Main credit goes to the multimedia institute (mi2/mama)
in Zagreb, Croatia, for constant support and good vibes, specifically
to m@rcell. Kiberpipa, Ljubljana, Slovenia also deserves gratitude for
providing a similar environment. The piksel festival and it's host, BEK,
in Bergen, Norway, plays a significant role in connecting me to an
environment where stuff like this can flourish. Shouts go out to my
posse in Weimar, Germany: subsignal, sumaato, KulturForschungsZentrum.
And last, because most important: thanks, Sylvi.


## So What?

That's all, folks. If you have comments, suggestions, need help, whatever,
write to:
	Daniel Turing <mail_AT_danielturing_DOT_com> (apply anti-spam logic)
	
Please also do so if you put libppgst to any interesting (or merely cool)
use. I'd really like to know.

