export const metadata = {
    title: "Карта сайта | MedScheduler",
    description: "Полная структура сайта MedScheduler с описанием всех страниц и функций",
};

export default function SiteMapPage() {
    return (
        <main className="prose prose-lg max-w-5xl mx-auto px-4 py-10">
            <h1>Карта сайта MedScheduler</h1>
            <p>Полная структура сайта с описанием всех страниц и их функциональности.</p>

            <hr />

            <h2>🏠 Основные страницы</h2>
            
            <div className="grid gap-6 md:grid-cols-2 not-prose">
                <div className="bg-white border border-gray-200 rounded-lg p-6 shadow-sm">
                    <h3 className="text-xl font-semibold text-main2 mb-3">Главная страница</h3>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/</code>
                    </p>
                    <p className="text-gray-700 mb-3">
                        Приветственная страница с поиском врачей, информацией о сервисе и основными разделами.
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Главный поиск по врачам и клиникам</li>
                        <li>• Информация о сервисе</li>
                        <li>• Раздел "Как это работает"</li>
                        <li>• Промо-блок мобильного приложения</li>
                    </ul>
                </div>

                <div className="bg-white border border-gray-200 rounded-lg p-6 shadow-sm">
                    <h3 className="text-xl font-semibold text-main2 mb-3">Поиск врачей</h3>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/search</code>
                    </p>
                    <p className="text-gray-700 mb-3">
                        Расширенный поиск врачей с множественными фильтрами и текстовым поиском.
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Текстовый поиск по ФИО, специальности</li>
                        <li>• Фильтры по региону и населенному пункту</li>
                        <li>• Фильтр по больницам и специальностям</li>
                        <li>• Сортировка по рейтингу</li>
                        <li>• Отображение результатов с возможностью записи</li>
                    </ul>
                </div>
            </div>

            <h2>🔐 Аутентификация</h2>
            
            <div className="grid gap-6 md:grid-cols-2 not-prose">
                <div className="bg-white border border-gray-200 rounded-lg p-6 shadow-sm">
                    <h3 className="text-xl font-semibold text-main2 mb-3">Вход в систему</h3>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/login</code>
                    </p>
                    <p className="text-gray-700 mb-3">
                        Страница авторизации для всех типов пользователей.
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Вход по номеру телефона и паролю</li>
                        <li>• Автоматическое перенаправление по ролям</li>
                        <li>• Восстановление пароля</li>
                    </ul>
                </div>

                <div className="bg-white border border-gray-200 rounded-lg p-6 shadow-sm">
                    <h3 className="text-xl font-semibold text-main2 mb-3">Регистрация</h3>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/registration</code>
                    </p>
                    <p className="text-gray-700 mb-3">
                        Регистрация новых пользователей (по умолчанию как пациентов).
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Форма с ФИО, телефоном и паролем</li>
                        <li>• Валидация данных</li>
                        <li>• Автоматическое создание профиля пациента</li>
                    </ul>
                </div>
            </div>

            <h2>👤 Личные кабинеты</h2>
            
            <div className="grid gap-6 md:grid-cols-1 not-prose">
                <div className="bg-white border border-gray-200 rounded-lg p-6 shadow-sm">
                    <h3 className="text-xl font-semibold text-main2 mb-3">Общий личный кабинет</h3>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/dashboard</code>
                    </p>
                    <p className="text-gray-700 mb-3">
                        Главная страница личного кабинета с перенаправлением по ролям.
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Автоматическое перенаправление на соответствующую страницу роли</li>
                        <li>• Отображение основной информации профиля</li>
                    </ul>
                </div>
            </div>

            <h3>🏥 Для пациентов</h3>
            <div className="grid gap-6 md:grid-cols-2 not-prose">
                <div className="bg-blue-50 border border-blue-200 rounded-lg p-6 shadow-sm">
                    <h4 className="text-lg font-semibold text-main2 mb-3">Профиль пациента</h4>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/patient</code>
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Просмотр и редактирование личных данных</li>
                        <li>• История обращений</li>
                        <li>• Настройки уведомлений</li>
                    </ul>
                </div>

                <div className="bg-blue-50 border border-blue-200 rounded-lg p-6 shadow-sm">
                    <h4 className="text-lg font-semibold text-main2 mb-3">Мои записи</h4>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/patient/appointments</code>
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Список всех записей к врачам</li>
                        <li>• Возможность отмены записи</li>
                        <li>• История посещений</li>
                        <li>• Оценка врачей</li>
                    </ul>
                </div>
            </div>

            <h3>👩‍⚕️ Для врачей</h3>
            <div className="grid gap-6 md:grid-cols-2 not-prose">
                <div className="bg-green-50 border border-green-200 rounded-lg p-6 shadow-sm">
                    <h4 className="text-lg font-semibold text-main2 mb-3">Кабинет врача</h4>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/doctor</code>
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Управление профилем и расписанием</li>
                        <li>• Просмотр записей пациентов</li>
                        <li>• Статистика приемов</li>
                        <li>• Настройки доступности</li>
                    </ul>
                </div>

                <div className="bg-green-50 border border-green-200 rounded-lg p-6 shadow-sm">
                    <h4 className="text-lg font-semibold text-main2 mb-3">Расписание врача</h4>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/doctor/schedule</code>
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Календарь приемов</li>
                        <li>• Управление рабочими часами</li>
                        <li>• Блокировка времени</li>
                        <li>• Экспорт расписания</li>
                    </ul>
                </div>
            </div>

            <h3>👨‍💼 Для младших администраторов</h3>
            <div className="grid gap-6 md:grid-cols-2 not-prose">
                <div className="bg-yellow-50 border border-yellow-200 rounded-lg p-6 shadow-sm">
                    <h4 className="text-lg font-semibold text-main2 mb-3">Панель мл. администратора</h4>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/junior-admin</code>
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Управление записями пациентов</li>
                        <li>• Просмотр расписания врачей</li>
                        <li>• Работа с листом ожидания</li>
                        <li>• Отчеты по загруженности</li>
                    </ul>
                </div>

                <div className="bg-yellow-50 border border-yellow-200 rounded-lg p-6 shadow-sm">
                    <h4 className="text-lg font-semibold text-main2 mb-3">Управление записями</h4>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/junior-admin/appointments</code>
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Создание/отмена записей</li>
                        <li>• Перенос записей</li>
                        <li>• Работа с экстренными случаями</li>
                        <li>• Уведомления пациентов</li>
                    </ul>
                </div>
            </div>

            <h3>🏛️ Для старших администраторов</h3>
            <div className="grid gap-6 md:grid-cols-2 not-prose">
                <div className="bg-red-50 border border-red-200 rounded-lg p-6 shadow-sm">
                    <h4 className="text-lg font-semibold text-main2 mb-3">Панель ст. администратора</h4>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/senior-admin</code>
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Управление пользователями</li>
                        <li>• Настройки системы</li>
                        <li>• Аналитика и отчеты</li>
                        <li>• Управление больницами</li>
                    </ul>
                </div>

                <div className="bg-red-50 border border-red-200 rounded-lg p-6 shadow-sm">
                    <h4 className="text-lg font-semibold text-main2 mb-3">Управление персоналом</h4>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/senior-admin/staff</code>
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Добавление врачей и администраторов</li>
                        <li>• Управление правами доступа</li>
                        <li>• Мониторинг активности</li>
                        <li>• Архивирование аккаунтов</li>
                    </ul>
                </div>
            </div>

            <h2>📚 Документация и информация</h2>
            
            <div className="grid gap-6 md:grid-cols-2 not-prose">
                <div className="bg-white border border-gray-200 rounded-lg p-6 shadow-sm">
                    <h3 className="text-xl font-semibold text-main2 mb-3">API Документация</h3>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/api-documentation</code>
                    </p>
                    <p className="text-gray-700 mb-3">
                        Полная документация всех API эндпоинтов сервера.
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Описание всех API методов</li>
                        <li>• Примеры запросов и ответов</li>
                        <li>• Коды ошибок и их значения</li>
                        <li>• Руководство по интеграции</li>
                    </ul>
                </div>

                <div className="bg-white border border-gray-200 rounded-lg p-6 shadow-sm">
                    <h3 className="text-xl font-semibold text-main2 mb-3">Карта сайта</h3>
                    <p className="text-gray-600 mb-3">
                        <strong>URL:</strong> <code>/site-map</code>
                    </p>
                    <p className="text-gray-700 mb-3">
                        Эта страница - полная структура сайта с описанием всех разделов.
                    </p>
                    <ul className="text-sm text-gray-600">
                        <li>• Навигация по всем страницам</li>
                        <li>• Описание функциональности</li>
                        <li>• Роли и права доступа</li>
                        <li>• Техническая информация</li>
                    </ul>
                </div>
            </div>

            <h2>🛠️ Техническая информация</h2>
            
            <div className="bg-gray-50 border border-gray-200 rounded-lg p-6 not-prose">
                <h3 className="text-xl font-semibold text-main2 mb-4">Стек технологий</h3>
                <div className="grid gap-4 md:grid-cols-3">
                    <div>
                        <h4 className="font-semibold mb-2 text-main2">Frontend</h4>
                        <ul className="text-sm text-gray-600">
                            <li>• Next.js 14</li>
                            <li>• React 18</li>
                            <li>• Tailwind CSS</li>
                            <li>• JavaScript ES6+</li>
                        </ul>
                    </div>
                    <div>
                        <h4 className="font-semibold mb-2 text-main2">Backend</h4>
                        <ul className="text-sm text-gray-600">
                            <li>• C++ сервер</li>
                            <li>• PostgreSQL база данных</li>
                            <li>• Docker контейнеризация</li>
                            <li>• REST API</li>
                        </ul>
                    </div>
                    <div>
                        <h4 className="font-semibold mb-2 text-main2">Desktop App</h4>
                        <ul className="text-sm text-gray-600">
                            <li>• Qt Framework</li>
                            <li>• C++</li>
                            <li>• Cross-platform UI</li>
                            <li>• Native performance</li>
                        </ul>
                    </div>
                </div>
            </div>

            <h2>🔒 Безопасность и роли</h2>
            
            <div className="bg-blue-50 border border-blue-200 rounded-lg p-6 not-prose">
                <h3 className="text-xl font-semibold text-main2 mb-4">Система ролей</h3>
                <div className="grid gap-4 md:grid-cols-4">
                    <div className="text-center">
                        <div className="bg-blue-100 rounded-full w-12 h-12 flex items-center justify-center mx-auto mb-2">
                            👤
                        </div>
                        <h4 className="font-semibold text-main2">Пациент</h4>
                        <p className="text-xs text-gray-600">Поиск врачей, запись на прием, управление записями</p>
                    </div>
                    <div className="text-center">
                        <div className="bg-green-100 rounded-full w-12 h-12 flex items-center justify-center mx-auto mb-2">
                            👩‍⚕️
                        </div>
                        <h4 className="font-semibold text-main2">Врач</h4>
                        <p className="text-xs text-gray-600">Управление расписанием, просмотр записей</p>
                    </div>
                    <div className="text-center">
                        <div className="bg-yellow-100 rounded-full w-12 h-12 flex items-center justify-center mx-auto mb-2">
                            👨‍💼
                        </div>
                        <h4 className="font-semibold text-main2">Мл. Админ</h4>
                        <p className="text-xs text-gray-600">Управление записями, работа с пациентами</p>
                    </div>
                    <div className="text-center">
                        <div className="bg-red-100 rounded-full w-12 h-12 flex items-center justify-center mx-auto mb-2">
                            🏛️
                        </div>
                        <h4 className="font-semibold text-main2">Ст. Админ</h4>
                        <p className="text-xs text-gray-600">Полное управление системой</p>
                    </div>
                </div>
            </div>

            <h2>📱 Мобильное приложение</h2>
            
            <div className="bg-gradient-to-r from-main to-main2 text-white rounded-lg p-6 not-prose">
                <h3 className="text-xl font-semibold mb-4">Qt Desktop Application</h3>
                <p className="mb-4">
                    Помимо веб-версии, доступно нативное десктопное приложение, созданное на Qt Framework.
                </p>                <ul className="text-sm">
                    <li>• Авторизация и регистрация пациентов</li>
                    <li>• Поиск врачей по специальности, региону и больнице</li>
                    <li>• Просмотр расписания врачей и запись на прием</li>
                    <li>• Управление своими записями (просмотр, отмена)</li>
                    <li>• Система ожидания при отсутствии свободных слотов</li>
                    <li>• Оставление отзывов и оценок врачам</li>
                    <li>• Панель управления для врачей (расписание, записи)</li>
                    <li>• Административные панели для младших и старших администраторов</li>
                    <li>• Управление больницами, врачами и пользователями</li>
                    <li>• Редактирование профиля и удаление аккаунта</li>
                </ul>
            </div>

            <div className="bg-yellow-50 border-l-4 border-yellow-400 p-4 mt-8">
                <p className="text-sm text-yellow-800">
                    <strong>Примечание:</strong> Некоторые страницы могут быть недоступны в зависимости от роли пользователя. 
                    Система автоматически перенаправляет пользователей на соответствующие их роли разделы.
                </p>
            </div>
        </main>
    );
}
