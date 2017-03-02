<!--TO SEE THE PHP YOU MUST RUN ON PHP SERVER-->
<!--LOCAL PHP5 SERVER: `php -S localhost:8080`-->

<!DOCTYPE html>
<html>

<head>
	<meta charset="UTF-8">
	<link rel="icon" type="image/png" href="calculator.png">
	<title>Simple Calculator</title>
</head>

<body>
	<form>
		<input type="text" name="num1" placeholder="First Number">
		<input type="text" name="num2" placeholder="Second Number">
		<select name="operator">
			<option>None</option>
			<option>Add</option>
			<option>Subtract</option>
			<option>Multiply</option>
			<option>Divide</option>
		</select>
		<br>
		<button type="submit" name="submit" value="submit">Calculate</button>
	</form>
	<p>THAT MAKES:</p>

	<?php
		if (isset($_GET['submit'])) {
			$result1 =  $_GET['num1'];
			$result2 =  $_GET['num2'];
			$operator =  $_GET['operator'];

			switch ($operator) {
				case "None":
					echo "No operator selected!";
					break;
				case "Add":
					echo $result1 + $result2;
					break;
				case "Subtract":
					echo $result1 - $result2;
					break;
				case "Multiply":
					echo $result1 * $result2;
					break;
				case "Divide":
					echo $result1 / $result2;
					break;
			}
		}
	?>

</body>

</html>