$('form.ajax').on('submit', function() {
	var that = $(this),
		url = that.attr('action'),
		method = that.attr('method'),
		data = {};

	that.find('[name]').each(funtion() {
		//console.log(value);
		var that = $(this),
			name = that.attr('name'),
			value = that.val();

		data[name] = value;
	});

	//console.log(data);
	$.ajax({
		url: url,
		type: method,
		data: data,
		success: function(response) {
			console.log(response);
		}
	});

	return false;
});