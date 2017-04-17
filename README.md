[![Gem Version](https://badge.fury.io/rb/sooth.svg)](http://badge.fury.io/rb/sooth)
[![Dependency Status](https://gemnasium.com/jasonhutchens/sooth.png)](https://gemnasium.com/jasonhutchens/sooth)
[![Codeship Status for jasonhutchens/sooth](https://codeship.com/projects/24d68130-e5b3-0132-369b-0addec64e5bc/status?branch=master)](https://codeship.com/projects/82075)

Sooth
=====

Sooth is a minimal stochastic predictive model. It is used by [MegaHAL](https://github.com/jasonhutchens/megahal), a learning chatterbot, and by [Typing Simulator](https://github.com/jasonhutchens/typing_simulator), a program that pretends to type text files like a human being.

Getting Started
---------------

The basic philisophy is to keep things simple. Sooth can make _predictions_ about _events_ that occur within some _context_.

A _context_ is just a number; it's up to you to make it significant. It might represent a word, or an array of words, or something else entirely. Sooth doesn't care. Likewise, an _event_ is a number, but doesn't need to represent the same kind of thing as a _context_. It's perfectly fine for a _context_ to be an array of words and an _event_ to be a single word, which is what MegaHAL does. Or a _context_ could be an array of characters and an _event_ could be a time in milliseconds, which is what TypingSimulator does. Or they could be something else entirely...

Look at the [API docs](http://www.rubydoc.info/gems/sooth/Sooth/Predictor) to get up to speed, and at the specs for example usage.

Copyright
---------

Copyright (c) 2017 Jason Hutchens. See UNLICENSE for further details.
