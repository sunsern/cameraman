#!/bin/bash
javac -cp . Feature.java
javac -cp . HSVFeature.java
javac -cp . RootFeature.java
javac -cp . Stage.java
javac -cp . ColorScorer.java
#
javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. MaxColorDetector.java
javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. RGB2HSV.java

