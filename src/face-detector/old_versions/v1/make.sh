#!/bin/bash
#
javac Candidate.java
javac Cascade.java
javac CascadeWithSkinColor.java
javac Feature.java
javac Stage.java
javac RectFeature.java
javac SkinScorer.java
javac IntegralImage.java
#
#javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. MaxFaceDetector.java
javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. MaxFaceDetectorSkin.java
javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. DrawBox.java
javac -cp /Applications/Max5/Cycling\ \'74/java/lib/max.jar:/Applications/Max5/Cycling\ \'74/java/lib/jitter.jar:. DisplayMessage.java
