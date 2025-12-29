
ThisBuild / scalaVersion := "2.12.10"

lazy val core = (projectMatrix in file("core"))
  .settings(
    name := "core",
  )
  .jvmPlatform(scalaVersions = Seq("2.12.10"))
  .jsPlatform(scalaVersions = Seq("2.12.10"))
  .nativePlatform(scalaVersions = Seq("2.12.10"))

libraryDependencies ++= Seq(
  "com.google.guava" % "guava" % "17.0",
  // other dependencies separated by commas
)
