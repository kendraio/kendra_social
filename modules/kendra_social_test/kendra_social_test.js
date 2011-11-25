/*
 * @file JavaScript for kendra_social_test.
 */

(function($) {

	// enumerate any ajax callbacks set
	Drupal.behaviors.enableFormItemsForAjaxForms = {
		attach : function(page, props) {
			if (!props.ajax)
				return;

			var options = {};
			for (prop in props.ajax) {
				if (props.ajax.hasOwnProperty(prop)) {
					options[prop] = $('#' + prop).val();
				}
			}
		}
	};

	Drupal.behaviors.kendraSocialTestAJAX = {
		attach : function(context, settings) {
		}
	};

	/**
	 * 
	 */
	var kendraSocialTestService = function(form) {
		var $form = $(form), url = $form.find('[name=url]').removeClass('required error').val(), content_type = $form.find('[name=content_type]').removeClass('required error').val(), method = $form.find('[name=method]').removeClass(
				'required error').val();
		var data = '';

		if (!url) {
			$form.find('[name=url]').addClass('required error');
			return false;
		} else if (!method) {
			$form.find('[name=method]').addClass('required error');
			return false;
		} else if (!content_type) {
			$form.find('[name=content_type]').addClass('required error');
			return false;
		}

		var params = [];
		$('[name^="params_fieldset\[param\]"]').each(function(i, s) {
			params.push($(s).val());
		});

		if (params.length > 0) {
			//data: JSON.stringify(params),
			data = params.join('&');
		} else {
			data = $form.find('[name=data]').val();
		}

		var success = function(data, textStatus, jqXHR) {
			var html = '';
			if (typeof jqXHR != 'undefined' && typeof jqXHR.responseXML != 'undefined') {
				html = jqXHR.responseText; // 
				$('#response').text(html);
			} else if (typeof data == 'string' || typeof prettyPrint == 'undefined') {
				html = data;
				$('#response').html(html);
            } else if (typeof data.embed == 'string') {
                html = data.embed;
                $('#response').html(html);
			} else {
				html = prettyPrint(data, {
					maxDepth : 6
				});
				$('#response').html(html);
			}
		}, error = function(jqXHR, textStatus, errorThrown) {
			var err = 'HTTP ' + [jqXHR.status, jqXHR.statusText].join(': ');
			if (typeof prettyPrint != 'undefined') {
				html = prettyPrint(errorThrown, {
					maxDepth : 6
				});
				$('#response').html(html);
			} else {
				$('#response').html(textStatus);
			}
		};
		
		var options = {
			context : '#response',
			//contentType : content_type,
			type : method,
			data : data,
			success : success,
			error : error
		};

		switch (content_type.split(/[\-/]/).pop()) {
		case 'json':
			options.dataType = 'json';
			break;
		case 'xml':
			options.dataType = 'xml';
			break;
		default:
			options.dataType = 'text';
		}

		try {
			$('#response').prepend('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');

			$.ajaxSetup({
				'error': function(e, jqXHR, ajaxSettings, thrownError) {
					if (typeof prettyPrint != 'undefined') {
						html = prettyPrint(e, {
							maxDepth : 6,
							maxStringLength : 100
						});
						$('#response').html(html);
					}
				},
				'complete':function(jqXHR, textStatus) {
					$('.ajax-progress', '#response').remove();
				}
				});
			$.ajax(url, options);
		} catch (e) {
			if (typeof prettyPrint != 'undefined') {
				html = prettyPrint(e, {
					maxDepth : 6,
					maxStringLength : 100
				});
				$('#response').html(html);
			} else {
				console.log(e);
			}
		}

		return false;
	}
	
	/**
	 * document ready handler - used here because Drupal Behaviors didn't behave
	 */
	$(document).ready(function() {

		if (typeof console == 'undefined') {
			console = {
				log : function(str) {
					alert(str);
				}
			};
		}

		$('#kendra-social-test-api').once('kendraSocialTestAJAX').submit(function() {
				return kendraSocialTestService(this);
		});
	});

})(jQuery);
