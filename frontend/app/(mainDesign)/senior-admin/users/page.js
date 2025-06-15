"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function SeniorAdminUsersPage() {
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState("");
    const [success, setSuccess] = useState("");
    const router = useRouter();

    // Список пользователей
    const [users, setUsers] = useState([]);

    // Поиск пользователей
    const [searchQuery, setSearchQuery] = useState("");
    const [filteredUsers, setFilteredUsers] = useState([]);

    // Фильтр по типу пользователя
    const [userTypeFilter, setUserTypeFilter] = useState("all"); // "all", "patient", "doctor", "junior", "senior"

    // Состояние для удаления пользователя
    const [deletingUser, setDeletingUser] = useState(null);
    const [confirmPhone, setConfirmPhone] = useState("");

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'senior') {
            router.push('/login');
            return;
        }

        // Загружаем список пользователей
        fetchUsers();
    }, [router]);

    useEffect(() => {
        // Фильтрация пользователей при изменении поискового запроса или типа пользователя
        let filtered = users;

        // Фильтрация по типу пользователя
        if (userTypeFilter !== "all") {
            filtered = filtered.filter(user => user.user_type === userTypeFilter);
        }

        // Фильтрация по поисковому запросу
        if (searchQuery) {
            const lowercaseQuery = searchQuery.toLowerCase();
            filtered = filtered.filter(
                user =>
                    (user.last_name && user.last_name.toLowerCase().includes(lowercaseQuery)) ||
                    (user.first_name && user.first_name.toLowerCase().includes(lowercaseQuery)) ||
                    (user.patronymic && user.patronymic.toLowerCase().includes(lowercaseQuery)) ||
                    user.phone.toLowerCase().includes(lowercaseQuery)
            );
        }

        setFilteredUsers(filtered);
    }, [searchQuery, userTypeFilter, users]);

    const fetchUsers = async () => {
        try {
            const response = await fetch('https://api.medscheduler.ru/get_users');
            const data = await response.json();

            if (response.ok && data.success) {
                setUsers(data.users);
                setFilteredUsers(data.users);
            } else {
                setError(data.error || "Ошибка при получении списка пользователей");
                // Используем пустой массив, если от сервера пришла ошибка
                setUsers([]);
                setFilteredUsers([]);
            }
        } catch (err) {
            console.error("Error fetching users:", err);
            setError("Не удалось подключиться к серверу");
            // Если соединение с сервером не удалось, используем пустой массив
            setUsers([]);
            setFilteredUsers([]);
        } finally {
            setLoading(false);
        }
    };

    const handleDeleteUser = async () => {
        if (!deletingUser) return;

        if (confirmPhone !== deletingUser.phone) {
            setError("Введенный номер телефона не совпадает с номером пользователя");
            return;
        }

        try {
            const response = await fetch(`https://api.medscheduler.ru/delete_user_by_phone?phone=${deletingUser.phone}`, {
                method: 'DELETE',
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setSuccess(`Пользователь ${deletingUser.last_name} ${deletingUser.first_name} успешно удален`);

                // Удаляем пользователя из списка
                setUsers(users.filter(user => user.phone !== deletingUser.phone));

                // Очищаем состояние удаления
                setDeletingUser(null);
                setConfirmPhone("");

                // Сбрасы��аем сообщение об успехе через 3 секунды
                setTimeout(() => {
                    setSuccess("");
                }, 3000);
            } else {
                setError(data.error || "Ошибка при удалении пользователя");
            }
        } catch (err) {
            console.error("Error deleting user:", err);
            setError("Не удалось подключиться к серверу");
        }
    };

    // Преобразование типа пользователя в читаемый текст
    const getUserTypeText = (type) => {
        switch (type) {
            case 'patient':
                return 'Пациент';
            case 'doctor':
                return 'Врач';
            case 'junior administrator':
                return 'Младший администратор';
            case 'senior administrator':
                return 'Старший администратор';
            default:
                return 'Неизвестно';
        }
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка данных...</div>
            </div>
        );
    }

    return (
        <div className="max-w-6xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Управление пользователями</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            {success && (
                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                    {success}
                </div>
            )}

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <div className="flex flex-col md:flex-row justify-between md:items-center mb-6">
                    <h2 className="text-xl font-semibold text-main2">Список пользователей</h2>                    <div className="mt-4 md:mt-0">
                        <div className="flex flex-wrap gap-2">
                            <button
                                key="all"
                                onClick={() => setUserTypeFilter("all")}
                                className={`px-3 py-1 rounded-md ${
                                    userTypeFilter === "all"
                                    ? "bg-main text-white"
                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                }`}
                            >
                                Все
                            </button>
                            <button
                                key="patient"
                                onClick={() => setUserTypeFilter("patient")}
                                className={`px-3 py-1 rounded-md ${
                                    userTypeFilter === "patient"
                                    ? "bg-main text-white"
                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                }`}
                            >
                                Пациенты
                            </button>
                            <button
                                key="doctor"
                                onClick={() => setUserTypeFilter("doctor")}
                                className={`px-3 py-1 rounded-md ${
                                    userTypeFilter === "doctor"
                                    ? "bg-main text-white"
                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                }`}
                            >
                                Врачи
                            </button>
                            <button
                                key="junior"
                                onClick={() => setUserTypeFilter("junior administrator")}
                                className={`px-3 py-1 rounded-md ${
                                    userTypeFilter === "junior administrator"
                                    ? "bg-main text-white"
                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                }`}
                            >
                                Мл. администраторы
                            </button>
                            <button
                                key="senior"
                                onClick={() => setUserTypeFilter("senior administrator")}
                                className={`px-3 py-1 rounded-md ${
                                    userTypeFilter === "senior administrator"
                                    ? "bg-main text-white"
                                    : "bg-gray-100 text-gray-700 hover:bg-gray-200"
                                }`}
                            >
                                Ст. администраторы
                            </button>
                        </div>
                    </div>
                </div>

                {/* Поиск пользователей */}
                <div className="mb-6">
                    <label htmlFor="search" className="block text-sm font-medium text-gray-700 mb-1">
                        Поиск пользователей
                    </label>
                    <input
                        type="text"
                        id="search"
                        value={searchQuery}
                        onChange={(e) => setSearchQuery(e.target.value)}
                        placeholder="Поиск по ФИО или телефону"
                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                    />
                </div>

                {/* Модальное окно для подтверждения удаления */}
                {deletingUser && (
                    <div className="mb-6 p-4 bg-red-50 border border-red-200 rounded-lg">
                        <h3 className="text-lg font-medium text-red-800 mb-4">
                            Удаление пользователя
                        </h3>
                        <p className="text-gray-700 mb-4">
                            Вы собираетесь удалить пользователя <span className="font-medium">{deletingUser.last_name} {deletingUser.first_name} {deletingUser.patronymic || ""}</span> ({getUserTypeText(deletingUser.user_type)}).
                            Это действие необратимо.
                        </p>
                        <p className="text-gray-700 mb-4">
                            Для подтверждения введите номер телефона пользователя: <span className="font-medium">{deletingUser.phone}</span>
                        </p>
                        <div className="mb-4">
                            <input
                                type="text"
                                value={confirmPhone}
                                onChange={(e) => setConfirmPhone(e.target.value)}
                                placeholder="Введите номер телефона"
                                className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-red-500"
                            />
                        </div>
                        <div className="flex gap-3">
                            <button
                                onClick={handleDeleteUser}
                                className="px-4 py-2 bg-red-600 text-white rounded hover:bg-red-700 transition-colors"
                            >
                                Удалить
                            </button>
                            <button
                                onClick={() => {
                                    setDeletingUser(null);
                                    setConfirmPhone("");
                                }}
                                className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                            >
                                Отмена
                            </button>
                        </div>
                    </div>
                )}

                {/* Список пользователей */}
                <div>
                    {filteredUsers.length === 0 ? (
                        <p className="text-gray-500 text-center py-4">Пользователи не найдены</p>
                    ) : (
                        <div className="overflow-x-auto">
                            <table className="min-w-full divide-y divide-gray-200">
                                <thead className="bg-gray-50">
                                    <tr>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            ФИО
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Телефон
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Тип пользователя
                                        </th>
                                        <th scope="col" className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">
                                            Действия
                                        </th>
                                    </tr>
                                </thead>
                                <tbody className="bg-white divide-y divide-gray-200">
                                    {filteredUsers.map((user) => (
                                        <tr key={user.user_id}>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm font-medium text-gray-900">
                                                    {user.last_name} {user.first_name} {user.patronymic || ""}
                                                </div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <div className="text-sm text-gray-500">{user.phone}</div>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap">
                                                <span className={`px-2 inline-flex text-xs leading-5 font-semibold rounded-full ${
                                                    user.user_type === 'patient' ? 'bg-blue-100 text-blue-800' :
                                                    user.user_type === 'doctor' ? 'bg-green-100 text-green-800' :
                                                    user.user_type === 'junior administrator' ? 'bg-yellow-100 text-yellow-800' :
                                                    'bg-purple-100 text-purple-800'
                                                }`}>
                                                    {getUserTypeText(user.user_type)}
                                                </span>
                                            </td>
                                            <td className="px-6 py-4 whitespace-nowrap text-sm font-medium">
                                                {user.user_type !== 'senior administrator' && (
                                                    <button
                                                        onClick={() => setDeletingUser(user)}
                                                        className="text-red-600 hover:text-red-900"
                                                    >
                                                        Удалить
                                                    </button>
                                                )}
                                            </td>
                                        </tr>
                                    ))}
                                </tbody>
                            </table>
                        </div>
                    )}
                </div>
            </div>

            <div className="bg-white shadow-md rounded-lg p-6 mb-6">
                <h2 className="text-xl font-semibold text-main2 mb-4">Информация об удалении пользователей</h2>
                <div className="prose prose-sm max-w-none text-gray-600">
                    <p>
                        <strong>Внимание:</strong> Удаление пользователя приведет к необратимому удалению всех связанных данных:
                    </p>
                    <ul>
                        <li>Для пациентов: все записи на прием, записи в листах ожидания и отзывы</li>
                        <li>Для врачей: их расписание, записи пациентов и отзывы</li>
                        <li>Для младших администраторов: привязки к больницам</li>
                    </ul>
                    <p>
                        Убедитесь, что вы действительно хотите удалить пользователя, прежде чем выполнять это действие.
                    </p>
                </div>
            </div>

            <div className="flex justify-between">
                <Link href="/senior-admin/junior-admins" className="text-main hover:text-main2 transition-colors">
                    ← Управление администраторами
                </Link>
                <Link href="/senior-admin/profile" className="text-main hover:text-main2 transition-colors">
                    Профиль →
                </Link>
            </div>
        </div>
    );
}
