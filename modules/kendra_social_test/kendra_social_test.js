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
			//console.log(options);
		}
	};

	Drupal.behaviors.kendraSocialTestAJAX = {
		attach : function(context, settings) {

			return false; // skip...
			$('form').once('kendraSocialTestAJAX').unbind('submit').bind('submit', function() {
				alert('submit');
				return false;
			}).each(function() {
				//console.log('attached', this);
				});
		}
	};

	$(document).ready(
			function() {

				if (typeof console == 'undefined') {
					console = {
						log : function(str) {
							alert(str);
						}
					};
				}

				var callback = function(response) {
					//console.log('got response', response);
					$('#response').html(response);
				};

				$('#kendra-social-test-api').once('kendraSocialTestAJAX').bind(
						'submit',
						function() {
							var $form = $(this), url = $form.find('[name=url]').removeClass('required error').val(), content_type = $form.find('[name=content_type]').removeClass('required error').val(), method = $form.find('[name=method]').removeClass(
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

							var options = {
								context : '#response',
								contentType : content_type,
								type : method,
								data : data,
								//dataType : 'json',
								success : callback
							};

							var ret = $.ajax(url, options);

							//console.log(options, ret);

							return false;
						});
			});

})(jQuery);
