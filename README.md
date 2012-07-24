Maxmod for FeOS
===============

Introduction
------------

This is a port of the Maxmod library to FeOS. There are several differences with the official library:

- Streaming is not available due to it not being compatible with FeOS by design.
- Filesystem/memory loading routines have been rewritten in C because the old ones were buggy and innefficent. As a side effect, you can use FAR with them (in fact it is recommended to do so).
- There are several new functions (look at maxmod9.h) for details:

    bool mmIsARM7Loaded();
    bool mmInitDefaultHandle(FILE* f, bool bTakeOwnership);
    bool mmInitDefaultFAR(far_t hFar, const char* aFileName);
    void mmUnloadSoundbank();

How to install
--------------

Clone this repository to <FeOSSDK>/userlib/maxmod. Then issue this command:

    make install

Maxmod FeOS app template
------------------------

A pre-made template that shows how to build a Maxmod app that loads audio assets from the FAR file system is included (look at the `template` folder).

Examples
--------

Several examples have been ported to FeOS, refer to the `examples` folder.
