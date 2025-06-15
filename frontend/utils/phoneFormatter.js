export const formatPhoneDisplay = (phone) => {
  // Удаляем все нецифровые символы
  const cleaned = phone.replace(/\D/g, '');
  
  // Если начинается с 8, заменяем на 7
  const normalized = cleaned.startsWith('8') ? '7' + cleaned.slice(1) : cleaned;
  
  // Форматируем для отображения
  if (normalized.length === 11 && normalized.startsWith('7')) {
    return `+7 (${normalized.slice(1, 4)}) ${normalized.slice(4, 7)}-${normalized.slice(7, 9)}-${normalized.slice(9, 11)}`;
  }
  
  return phone;
};

export const formatPhoneForAPI = (phone) => {
  // Удаляем все нецифровые символы, кроме возможного плюса в начале
  const cleaned = phone.replace(/[^\\d+]/g, '');
  
  // Если начинается с 8, заменяем на +7
  if (cleaned.startsWith('8')) {
    return '+7' + cleaned.slice(1);
  }
  // Если не начинается с +, но это российский номер (после удаления нецифровых символов осталась корректная длина)
  // и начинается с 7, добавляем +
  if (!cleaned.startsWith('+') && cleaned.startsWith('7') && cleaned.length === 11) {
    return '+' + cleaned;
  }
  // Если уже есть плюс и корректная длина (например, +79991234567)
  if (cleaned.startsWith('+7') && cleaned.length === 12) {
    return cleaned;
  }
  // Возвращаем "как есть", если формат уже с плюсом или не соответствует ожиданиям для автоматической коррекции
  // Валидация должна отсеять некорректные номера до этого шага.
  return cleaned; 
};

export const validatePhone = (phone) => {
  const forAPI = formatPhoneForAPI(phone); // Сначала приводим к формату API
  // Проверяем формат +7 и 10 цифр после
  return /^\\+7\\d{10}$/.test(forAPI);
};