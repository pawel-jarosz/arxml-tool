# ARXML Tool

## Overview

The repository contains a library and tool based on it which
supports edition and exploring AUTOSAR model files.

**IMPORTANT**! It does not verify model against scheme files or **AUTOSAR** specification.
It works on XML files with the basic rules observed on the *ARXML* files such as:

* The XML root is ```<AUTOSAR>``` with additional information about schema,
* Leaf ```AUTOSAR``` contains only one element ```<AR-PACKAGES>```,
* Leaf ```AR-PACKAGES``` contains multiple (>1) entries of ```<AR-PACKAGE>```,
* Leaf ```AR-PACKAGE``` contains leafs ```<AR-PACKAGES>``` or ```<ELEMENTS>```,
* Leaf ```ELEMENTS``` contains named elements.

Elements can be named, *just* composite or simple string. The ```/test``` directory contains example model with 
similar structure to  real **AUTOSAR** model.

### Library

```libarxml``` implements following feature:

* parsing *AUTOSAR model*,
* dumping *AUTOSAR model*,
* exploring model tree,
* build model from source code ```// TODO```.

### Tool

* show *arxml model* in more human-readable version, similar like
```tree``` tool
* exploring model:
  * find definition of the concrete model leaf 
  * find references to the model leaf
  * find all entries by element tag.
* refactoring ```// TODO```

## Build

### Native compilation

* C++20 compliant compiler,
* CMake
* Libraries:
  * ```Boost```
  * ```GTest\GMock```
  * ```TinyXML2```

### Docker

```// TODO```

## Manual

```// TODO```
