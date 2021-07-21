# timming

[calculations](https://docs.google.com/spreadsheets/d/1sW6OSL6_QKulB7d0tieEy35BXmXtSeJSMk4KoMCUnbY/edit?usp=sharing)

## back from right to left

Before:

```
17:09:38.768 -> moveStart | start
17:09:38.815 -> mSliCalcSteps | deg=7650, t=1000000, tBySt=10625000
17:09:38.815 -> mRotCalcSteps | degrees=540.00, t=10625000, tBySt=10625000
17:09:52.842 -> moveStart | stop
17:09:52.842 -> moveStop
```

After:

```
15:00:55.603 -> moveSetup | d=1000
15:00:55.603 -> moveStart | start
15:00:55.650 -> mSliCalcSteps | deg=7560, t=1000000, tBySt=10500000
15:00:55.650 -> mRotCalcSteps | degrees=540.00, t=10500000, tBySt=10500000
15:01:11.392 -> moveStart | stop
```


## 10 sec / 16 sec

Initial:

```
18:23:44.293 -> moveStart | start
18:23:44.340 -> mSliCalcSteps | deg=-7560, t=10000000, tBySt=10000000
18:23:44.386 -> mRotCalcSteps | degrees=-540.00, t=10000000, tBySt=10000000
18:24:00.090 -> moveStart | stop
18:24:00.090 -> moveStop
```

A:

```
15:01:28.685 -> moveSetup | d=16000
15:01:28.685 -> ^ r=1.00, f=3486
15:01:28.731 -> ^ d=10514
15:01:28.731 -> moveStart | start
15:01:28.778 -> mSliCalcSteps | deg=-7560, t=10514000, tBySt=10514000
15:01:28.825 -> mRotCalcSteps | degrees=-540.00, t=10514000, tBySt=10514000
15:01:44.462 -> moveStart | stop
```

B:

```
21:47:36.011 -> moveSetup | d=16000
21:47:36.011 -> ^ r=1.00, f=5276, d=10724
21:47:36.011 -> moveStart | start
21:47:36.057 -> mSliCalcSteps | deg=-7560, t=10724000, tBySt=10724000
21:47:36.103 -> mRotCalcSteps | degrees=-540.00, t=10724000, tBySt=10724000
21:47:52.106 -> moveStart | stop
```

## 30 sec

Initial:

```
17:05:04.668 -> moveStart | start
17:05:04.715 -> mSliCalcSteps | deg=-7560, t=30000000, tBySt=30000000
17:05:04.762 -> mRotCalcSteps | degrees=-540.00, t=30000000, tBySt=30000000
17:05:39.782 -> moveStart | stop
17:05:39.782 -> moveStop
```

A:

```
15:02:27.455 -> moveSetup | d=30000
15:02:27.455 -> ^ r=0.99, f=3472
15:02:27.501 -> ^ d=24528
15:02:27.501 -> moveStart | start
15:02:27.501 -> mSliCalcSteps | deg=-7560, t=24528000, tBySt=24528000
15:02:27.548 -> mRotCalcSteps | degrees=-540.00, t=24528000, tBySt=24528000
15:02:57.247 -> moveStart | stop
```

B:

```
21:52:00.534 -> moveSetup | d=30000
21:52:00.534 -> ^ r=0.99, f=5255, d=24745
21:52:00.580 -> moveStart | start
21:52:00.580 -> mSliCalcSteps | deg=-7560, t=24745000, tBySt=24745000
21:52:00.626 -> mRotCalcSteps | degrees=-540.00, t=24745000, tBySt=24745000
21:52:30.353 -> moveStart | stop
```

## 60 sec = 1 min

Initial:

```
17:07:47.474 -> moveStart | start
17:07:47.524 -> mSliCalcSteps | deg=-7560, t=60000000, tBySt=60000000
17:07:47.571 -> mRotCalcSteps | degrees=-540.00, t=60000000, tBySt=60000000
17:08:52.455 -> moveStart | stop
17:08:52.455 -> moveStop
```

A:

```
15:03:55.895 -> moveSetup | d=60000
15:03:55.895 -> ^ r=0.98, f=3443
15:03:55.942 -> ^ d=54557
15:03:55.942 -> moveStart | start
15:03:55.942 -> mSliCalcSteps | deg=-7560, t=54557000, tBySt=54557000
15:03:55.989 -> mRotCalcSteps | degrees=-540.00, t=54557000, tBySt=54557000
15:04:55.547 -> moveStart | stop
```

B:

```
21:49:53.518 -> moveSetup | d=60000
21:49:53.518 -> ^ r=0.98, f=5211, d=54789
21:49:53.564 -> moveStart | start
21:49:53.610 -> mSliCalcSteps | deg=-7560, t=54789000, tBySt=54789000
21:49:53.656 -> mRotCalcSteps | degrees=-540.00, t=54789000, tBySt=54789000
21:50:53.410 -> moveStart | stop
```

## 300 sec = 5 min

Initial:

```
17:11:06.162 -> moveStart | start
17:11:06.162 -> mSliCalcSteps | deg=-7560, t=300000000, tBySt=300000000
17:11:06.209 -> mRotCalcSteps | degrees=-540.00, t=300000000, tBySt=300000000
17:16:10.986 -> moveStart | stop
17:16:10.986 -> moveStop
```

A:

```
15:05:43.032 -> moveSetup | d=300000
15:05:43.032 -> ^ r=0.92, f=3210
15:05:43.079 -> ^ d=294790
15:05:43.079 -> moveStart | start
15:05:43.126 -> mSliCalcSteps | deg=-7560, t=294790000, tBySt=294790000
15:05:43.173 -> mRotCalcSteps | degrees=-540.00, t=294790000, tBySt=294790000
15:10:42.873 -> moveStart | stop
```

B:

```
21:53:14.342 -> moveSetup | d=300000
21:53:14.342 -> ^ r=0.92, f=4858, d=295142
21:53:14.342 -> moveStart | start
21:53:14.389 -> mSliCalcSteps | deg=-7560, t=295142000, tBySt=295142000
21:53:14.434 -> mRotCalcSteps | degrees=-540.00, t=295142000, tBySt=295142000
21:58:14.469 -> moveStart | stop
```

## 600 sec = 10 min

Initial:

```
17:18:41.687 -> moveStart | start
17:18:41.687 -> mSliCalcSteps | deg=-7560, t=600000000, tBySt=600000000
17:18:41.733 -> mRotCalcSteps | degrees=-540.00, t=600000000, tBySt=600000000
17:28:46.352 -> moveStart | stop
17:28:46.352 -> moveStop
```

A:

```
5:11:34.406 -> moveSetup | d=600000
15:11:34.406 -> ^ r=0.83, f=2918
15:11:34.453 -> ^ d=595082
15:11:34.453 -> moveStart | start
15:11:34.500 -> mSliCalcSteps | deg=-7560, t=595082000, tBySt=595082000
15:11:34.547 -> mRotCalcSteps | degrees=-540.00, t=595082000, tBySt=595082000
15:21:34.309 -> moveStart | stop
```

## 900 sec = 15 min

Initial:

```
17:30:15.199 -> moveStart | start
17:30:15.246 -> mSliCalcSteps | deg=-7560, t=900000000, tBySt=900000000
17:30:15.246 -> mRotCalcSteps | degrees=-540.00, t=900000000, tBySt=900000000
17:45:19.823 -> moveStart | stop
17:45:19.823 -> moveStop
```

A:

```
15:23:02.804 -> moveSetup | d=900000
15:23:02.804 -> ^ r=0.75, f=2626
15:23:02.851 -> ^ d=895374
15:23:02.851 -> moveStart | start
15:23:02.851 -> mSliCalcSteps | deg=-7560, t=895374000, tBySt=895374000
15:23:02.945 -> mRotCalcSteps | degrees=-540.00, t=895374000, tBySt=895374000
15:38:02.933 -> moveStart | stop
```

## 60 sec = 1 min + smoothing 1

```
20:57:21.924 -> moveSetup | d=60000
20:57:21.924 -> ^ r=0.98, f=5211, d=54789
20:57:21.971 -> moveStart | start
20:57:21.971 -> mSliCalcSteps | deg=-7560, t=54789000, tBySt=129614808
20:59:34.574 -> moveStart | stop
```