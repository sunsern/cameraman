#!/bin/bash
#
javac Feature.java
javac Stage.java
javac HSVFeature.java
javac SkinColorFeature.java
javac RectFeature.java
javac Candidate.java
javac Cascade.java
javac IntegralImage.java
javac SkinDetector.java
javac ColorScorer.java
#
javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. MaxFaceDetector.java
javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. DrawBox.java
javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. DisplayMessage.java
