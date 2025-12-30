import scala.sys.process._

object Main {
  def main(args: Array[String]): Unit = {
    val command = "python3 simple-print-script.py"
    val result = command.!!
    val segment_list = result.split("\n").toList
    for(i <- segment_list.indices) {
      println(s"$i : ${segment_list(i)}")
    }
  }
}