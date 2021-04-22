# Implementation Scheme for Last-Touch Prediction over MESI 2-Level

## [MESI Two-Level](https://www.gem5.org/documentation/general_docs/ruby/MESI_Two_Level/)

https://www.gem5.org/documentation/general_docs/ruby/MESI_Two_Level/



## Changes to `.sm`

### Make Trace
- Capture manadatoryRequest Load and Store
  - place into trace (list for now?)

### Events
- [x] Add internal events
  - [x] Load_Evict
  - [x] Store_Evict

#### LTP can happen @

| State | Rd/Wr  | Action       | Destination   |
|-------|--------|--------------|---------------|
| M     | X      | Writeback    | M_I / I       |
| S     | Wr     | Writeback    | M_I / I       |
| S     | Rd     | Evict        | `SI` / I      |
| E     | Wr     | Writeback    | M_I / I       |
| E     | Rd     | Evict        | `EI` / I      |
| I     | Wr     | Fetch, WB    | IM / M_I / I  |
| I     | Rd     | Fetch, Evict | IS / SI / I   |

<br>

> **TODO** add actions for transient states

### Structures
- Add (external?) structure for traceTable
  - with functions to process access to given addr
  - function to check ltp match? --- implemented later

### Transitions
- decide stalls
- complete state machine


## TODO

- add disctinction (using a true/false) for completed traces and L1-Replacements in the middle of a ongoing trace
  - resulting in trace discard.

- Overload endTrace function : one without PC(invalids and replacements) and the other with current PC(load evict and store evict).


