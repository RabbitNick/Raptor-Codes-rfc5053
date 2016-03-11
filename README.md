# rfc5053
This project is about Raptor Codes. We implemented rfc 5053(R10) Raptor Codes in C++. More detail about rfc 5053 please see : (rfc5053 :https://tools.ietf.org/html/rfc5053)


Install the Pre-Requisites:

@Development Tools(need for compilation)

	*g++
	*git
	*make
	*cmake


@Libraries(need for runtime and compilation)

	*boost 1.52



Install Dependencies:

We implemented RFC5053 on Ubuntu 14.04 and do not test it on other OSs. If you use Ubuntu 14.04, you can directly install the dependencies.

Trusty Tahr(14.04)

	sudo apt-get -y install cmake g++ git-core libboost1.55-all-dev

Of course, you can download this source codes of dependencies and install them.




Description:
RFC5053 describes a  systematic rateless codes. The first k source symbols are the first k encoded symbols. After k encoded symbols, there are parity symbols for recovering missing source symbols. You can choose a k between 4 and 8192.

For conveniently implementation, We did not implement the Raptor Codes decoder in bits matrix reference to the third and fourth phase of effective decoding of RFC5053. Whereas, that is easy to code in bits matrix using bitset in STL. 



Note: Choosing some k numbers can not work, because the encoding matrix is not full rank. We will figure out this problem later. If you have any ideas, please feel free to contact with us.

**************************************
Ben Zhang: zhangbjb at gmail dot com

Nick Chan: nick.chitchan at gmail dot com

**************************************



